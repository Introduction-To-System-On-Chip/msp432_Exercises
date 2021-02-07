#include "printf.h"

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/*
 * UART Configuration Parameter.
 * These are the configuration parameters to make the eUSCI A UART module
 * operate at a 9600 baud rate
 * These values are from the online calculator and depends on the clock frequency used:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/
 * You can use minicom -D /dev/ttyACM0 -b 9600
 */
const eUSCI_UART_ConfigV1 uartConfigAclk =
{
    EUSCI_A_UART_CLOCKSOURCE_ACLK, // ACLK Clock Source
    3,                             // clockPrescalar
    0,                             // firstModReg
    146,                           // secondModReg
    EUSCI_A_UART_NO_PARITY,
    EUSCI_A_UART_LSB_FIRST,
    EUSCI_A_UART_ONE_STOP_BIT,
    EUSCI_A_UART_MODE,
    EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION
};

const eUSCI_UART_ConfigV1 uartConfigSmclk =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK, // SMCLK Clock Source if used with DCO 12MHz
    // MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    78,                             // clockPrescalar
    2,                             // firstModReg
    0,                           // secondModReg
    EUSCI_A_UART_NO_PARITY,
    EUSCI_A_UART_LSB_FIRST,
    EUSCI_A_UART_ONE_STOP_BIT,
    EUSCI_A_UART_MODE,
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};

void setupPrint(void)
{
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3,
            GPIO_PRIMARY_MODULE_FUNCTION);
    
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfigAclk);
    
    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);

    // Interrupt enable Master must be called: MAP_Interrupt_enableMaster();
    lprintf(EUSCI_A0_BASE, "PRINT> Initialized.\r\n");
}
