#include "printf.h"
#include <stdint.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

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
      /* TODO: Toggle P2.2 LED */
    }
    else if (status & GPIO_PIN4)
    {
        /* TODO: Increase CCR1 of the timer */
    }

    /* TODO: Clear the interrupt otherwise the Handler will be called again */
}

static void initLedAndButton(void)
{
    /* TODO: Setup RED LED P2.2 */

    /* TODO: Setup the Button P1.1 with interrupt. */

    /* TODO: Setup the Button P1.4 with interrupt. */


    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
                                                GPIO_PIN0,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
}

static void initTimer(void)
{
    const Timer_A_UpModeConfig configA1 =
    {
      // TODO: Fill this structure
    };

    const Timer_A_CompareModeConfig compareConfigA1 =
    {
      // TODO: fill this structure
    };

    /* TODO: Init TimerA. configure as Up Mode and start the counter */
}

int main(void)
{
    /* Stop watchdog timer. */
    WDT_A_hold(WDT_A_BASE);

    /* TODO: Init Clocks */

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

    initLedAndButton();
    initTimer();

    /* Map P2.0 to output timer TA1 */
    const uint8_t portMap[] = { PMAP_TA1CCR1A };

    PMAP_configurePorts(portMap,
                        PMAP_P2MAP,
                        sizeof(portMap),
                        PMAP_DISABLE_RECONFIGURATION);

    /* Enable all interrupt on PORT 1 */
    Interrupt_enableInterrupt(INT_PORT1);
    /* Once everything is setup, enable MCU interrupts. */
    Interrupt_enableMaster();

    while(1)
    {
        PCM_gotoLPM0();
    }

    return 0;
}
