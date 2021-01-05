#include "printf.h"
#include <stdint.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define C_GPIO_PORT1_BUTON_LED_PIN GPIO_PIN2
#define C_PWM_CCR0 0x2000
#define C_PWM_CCR1 0x2000
#define C_PWM_STEP 0x100

/*
 * Redefine the IRQ Handler for PORT1 interrupts.
 * Names of the Handler can be found in startup_msp432p401r_gcc.c
 */
void PORT1_IRQHandler(void)
{
    uint16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);

    /* Filter only for the P1.1 interrupt which is from the button. */
    if(status & GPIO_PIN1)
    {
        GPIO_toggleOutputOnPin(GPIO_PORT_P2, C_GPIO_PORT1_BUTON_LED_PIN);
    }
    else if (status & GPIO_PIN4)
    {
        /* Increase */
        uint16_t ccr1 = Timer_A_getCaptureCompareCount(
                TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);

        ccr1 += C_PWM_STEP;
        if (ccr1 > C_PWM_CCR0)
        {
            ccr1 = 0;
        }

        Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, ccr1);

    }

    /*
     * Always clear the interrupt otherwise the Handler will be
     * called over and over.
     */
    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
}

void initButton(void)
{
    /* Setup RED LED P2.0 */
    GPIO_setAsOutputPin(GPIO_PORT_P2, C_GPIO_PORT1_BUTON_LED_PIN);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, C_GPIO_PORT1_BUTON_LED_PIN);

    /* Setup the Button P1.1 with interrupt. */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
}

void initTimer(void)
{
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
                                                GPIO_PIN0,
                                                GPIO_PRIMARY_MODULE_FUNCTION);

    /* Setup the Button P1.1 with interrupt. */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);

    const  Timer_A_UpModeConfig configA1 =
    {
      TIMER_A_CLOCKSOURCE_SMCLK,     // SMCLK  clock  source
      TIMER_A_CLOCKSOURCE_DIVIDER_2, // SMCLK / 2 = 6MHz
      C_PWM_CCR0,                    // Value  in CCR0
      TIMER_A_TAIE_INTERRUPT_DISABLE,      // No interrupt when counter is 0
      TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, // Enable  CCR0  interrupt
      TIMER_A_DO_CLEAR
    };

    Timer_A_CompareModeConfig compareConfigA1 =
    {
      TIMER_A_CAPTURECOMPARE_REGISTER_1, // Use CCR1
      TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
      TIMER_A_OUTPUTMODE_TOGGLE_RESET,
      C_PWM_CCR1 // CCR1
    };


    Timer_A_initCompare(TIMER_A1_BASE, &compareConfigA1);
    Timer_A_configureUpMode(TIMER_A1_BASE, &configA1);

    //Interrupt_enableInterrupt(INT_TA1_0);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}

int main(void)
{
    /* Stop watchdog timer. */
    WDT_A_hold(WDT_A_BASE);

    /* Setting DCO to 12MHz */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
    MAP_CS_initClockSignal(CS_MCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    setupPrint();

    uint32_t aclkFreq = CS_getACLK();
    uint32_t mclkFreq = CS_getMCLK();
    uint32_t smclkFreq = CS_getSMCLK();
    uint32_t hsmclkFreq = CS_getHSMCLK();
    uint32_t bclkFreq = CS_getBCLK();

    lprintf(EUSCI_A0_BASE,
            "Clock:\r\n"
            "\taclkFreq: 0x%x\r\n"
            "\tmclkFreq: 0x%x\r\n"
            "\tsmclkFreq: 0x%x\r\n"
            "\thsmclkFreq: 0x%x\r\n"
            "\tbclkFreq: 0x%x\r\n",
            aclkFreq, mclkFreq, smclkFreq, hsmclkFreq, bclkFreq);

    initButton();
    initTimer();

    /* Map P2.0 to output timer TA1 */
    const uint8_t portMap[] =
    {
      PMAP_TA1CCR1A // P2.0
    };

    PMAP_configurePorts(portMap,
                        PMAP_P2MAP,
                        sizeof(portMap),
                        PMAP_DISABLE_RECONFIGURATION);

    /* Enable all interrupt on PORT 1 */
    Interrupt_enableInterrupt(INT_PORT1);
    /* Once everything is setup, enable interrupts. */
    Interrupt_enableMaster();

    while(1)
    {
        PCM_gotoLPM0();
    }

    return 0;
}
