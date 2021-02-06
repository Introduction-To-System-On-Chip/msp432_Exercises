#include "printf.h"
#include <stdint.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define TIMER_FREQUENCY_DIVIDER TIMER_A_CLOCKSOURCE_DIVIDER_64
#define TIMER_REAL_FREQUENCY (CS_getACLK() / TIMER_FREQUENCY_DIVIDER)

typedef struct {
  uint16_t seconds;
  uint16_t milliseconds;
} currentTime_t;

static void initTimer(void)
{
    const Timer_A_ContinuousModeConfig timerConfig =
    {
        TIMER_A_CLOCKSOURCE_ACLK,       // ACLK Clock Source
        TIMER_FREQUENCY_DIVIDER,
        TIMER_A_TAIE_INTERRUPT_DISABLE, // Disable interrupts
        TIMER_A_DO_CLEAR                // Clear counter
    };

    Timer_A_configureContinuousMode(TIMER_A1_BASE, &timerConfig);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
}

static currentTime_t getTime(void)
{
    currentTime_t time;
    uint16_t value = Timer_A_getCounterValue(TIMER_A1_BASE);

    time.seconds = (value / TIMER_REAL_FREQUENCY);
    time.milliseconds = ((value * 1000) / TIMER_REAL_FREQUENCY) % 1000;

    return time;
}

static void printTime(void)
{
    currentTime_t time = getTime();
    lprintf(EUSCI_A0_BASE,
            "Time: %us %ums\r\n",
            time.seconds,
            time.milliseconds);
}

static void printTimeDifference(currentTime_t startTime)
{
    currentTime_t currentTime = getTime();
    uint16_t secondsElapsed = currentTime.seconds - startTime.seconds;
    uint16_t millisecondsElapsed = currentTime.milliseconds - startTime.milliseconds;

    if (currentTime.milliseconds < startTime.milliseconds)
    {
        millisecondsElapsed = startTime.milliseconds - currentTime.milliseconds;
    }

    lprintf(EUSCI_A0_BASE,
            "Time Elapsed: %us %ums\r\n",
            secondsElapsed,
            millisecondsElapsed);
}

static void setupButton(void)
{
    /* Set Button P1.1 as input */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);

    /* Enable P1 interrupt for Pin1 */
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    /* Enable all interrupt of PORT1 */
    MAP_Interrupt_enableInterrupt(INT_PORT1);
}

int main(void)
{
    /* Stop watchdog timer. So that it does not impact debugging */
    WDT_A_hold(WDT_A_BASE);

    MAP_CS_initClockSignal(CS_MCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    //MAP_CS_initClockSignal(CS_MCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_8);
    //MAP_CS_initClockSignal(CS_MCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_1);
    /*
     * CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_1_5); // CS_DCO_FREQUENCY_12
     * MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
     */

    setupPrint();
    initTimer();
    setupButton();

    /*
     * From the DriverLib User guide: "If the user calls a low power entry
     *  function that disables a clock source while an active peripheral
     *  is accessing the clock source, by default MSP432 will not allow
     *  the transition."
     *  This is the case of the code, where the timer is accessing the
     *  clock source.
     */
    MAP_PCM_enableRudeMode();

    /* Enabling all interrupts */
    MAP_Interrupt_enableMaster();

    printTime();

    while(1)
    {
        currentTime_t beforeTime = getTime();

        for (volatile int i = 0; i < 5000; i++)
        {
            // Simulation of a time consuming code..
        }

        printTimeDifference(beforeTime);

        MAP_PCM_gotoLPM3();
    }

    return 0;
}

void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
    /* No need to parse which interrupt as only the button is configured */
}

