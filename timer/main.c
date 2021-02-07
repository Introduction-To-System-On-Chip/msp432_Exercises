#include "printf.h"
#include <stdint.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

typedef struct {
  uint16_t seconds;
  uint16_t milliseconds;
} currentTime_t;

static void initTimer(void)
{
    /* TODO: Add the code to init the timer */
}

static currentTime_t getTime(void)
{
    currentTime_t time;

    /* TODO: Fill the proper values */
    time.seconds = 0;
    time.milliseconds = 0;

    return time;
}

static void printTime(void)
{
    currentTime_t time = getTime();
    lprintf(EUSCI_A0_BASE, "Time: %us %ums\r\n", time.seconds, time.milliseconds);
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

    /* TODO: Initialize the clock systems */

    initTimer();

    setupPrint();
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

/* GPIO ISR */
void PORT1_IRQHandler(void)
{
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
    /* No need to parse which interrupt as only the button is configured */
}
