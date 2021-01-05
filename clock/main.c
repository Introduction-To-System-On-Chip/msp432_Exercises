#include "printf.h"

/* To have the definition of the driverlib functions. */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>

int main(void)
{
    /* Stop watchdog timer. */
    WDT_A_hold(WDT_A_BASE);

    /* Setting DCO to 12MHz. */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    /* Setup the UART to configure lprintf function. */
    setupPrint();

    /* Set Port 1 Pin 0 (P1.0) direction to Output. */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Infinite loop. */
    while(1)
    {
        lprintf(EUSCI_A0_BASE, "Loop.\n");

        /* Toggle P1.0 output */
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

        /* Add a software delay. */
        for(volatile unsigned int i = 0; i < 100000U; i++);
    }

    /* Unreachable code.. */
    return 0;
}
