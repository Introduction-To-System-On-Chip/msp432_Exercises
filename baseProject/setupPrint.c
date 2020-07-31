#include "printf.h"

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/*
 * UART Configuration Parameter.
 * These are the configuration parameters to make the eUSCI A UART module
 * operate at a 9600 baud rate for a DCO at 12MHz.
 * These values are from the online calculator:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/
 * You can use minicom -D /dev/ttyACM0 -b 9600
 */
const eUSCI_UART_ConfigV1 uartConfig =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK, // SMCLK Clock Source
    78,                             // clockPrescalar BRDIV = 78
    2,                              // firstModReg UCxBRF = 2
    0,                              // secondModReg UCxBRS = 0
    EUSCI_A_UART_NO_PARITY,
    EUSCI_A_UART_LSB_FIRST,
    EUSCI_A_UART_ONE_STOP_BIT,
    EUSCI_A_UART_MODE,
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};

void setupPrint(void)
{
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P1,
            GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3,
            GPIO_PRIMARY_MODULE_FUNCTION);
    
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    
    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);
    
    MAP_Interrupt_enableMaster();   

    lprintf(EUSCI_A0_BASE, "PRINT> Initialized\r\n.");
}
