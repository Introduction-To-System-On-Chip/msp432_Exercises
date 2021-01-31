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
    lprintf(EUSCI_A0_BASE, "Time: %u.%u\r\n", time.seconds, time.milliseconds);
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
            "Time Elapsed: %u.%u\r\n",
            secondsElapsed,
            millisecondsElapsed);
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
    initTimer();

    lprintf(EUSCI_A0_BASE,
            "Clocks:\r\n"
            "\taclkFreq: %uHz\r\n"
            "\tsmclkFreq: %uHz\r\n",
            CS_getACLK(), CS_getMCLK());

    printTime();

    while(1)
    {
        currentTime_t beforeTime = getTime();

        for (volatile int i = 0; i < 5000; i++)
        {
            // Simulation of a time consuming code..
        }

        printTimeDifference(beforeTime);
    }

    return 0;
}
