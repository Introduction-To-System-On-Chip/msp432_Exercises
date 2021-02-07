# PWM Led

## Goals

- Use a PWM to change the intensity of a LED
- Use driverLib functions to configure a button, LED and Timer

# Instructions

The list of functions and defines of the driverLib can be found at:
`https://software-dl.ti.com/simplelink/esd/simplelink_msp432_sdk/3.30.00.13/docs/driverlib/msp432p4xx/html/modules.html`

Open the main.c and fill in the TODOs in the code to:

- Configure the clock MCLK and SMCLK using `CS_initClockSignal` and
  `CS_setDCOCenteredFrequency`.

- Configure the buttons as input and LED as output. Use `GPIO_setAsOutputPin`,
  `GPIO_setOutputLowOnPin` and `GPIO_setAsInputPinWithPullUpResistor`.
  
- Configure the interrupt for the button. Use `GPIO_interruptEdgeSelect` and
  `GPIO_enableInterrupt`.

- Init the timer to configure a PWM. Use Timer A1 with an input clock of 6MHz.
  Set a default value in the Capture and Compare register 0 (CCR0). Use
  the define `C_PWM_CCR0`.

- Configure the Timer as upmode and start it. Do not enable interrupts.

- In the `PORT1_IRQHandler`, when button P1.1 is pressed, LED P2.1 should be
  toggled. When button P1.4 is pressed, the Capture and Compare register 1 of
  the configured timer has to be incremented by `C_PWM_STEP`. If the CCR1 is
  greater than `C_PWM_CCR0`, it should be restarted to 0.
  Make sure to acknolege the interrupt flag.

When the program is started, the LED intensity should change when the PWM of the
timer is increased as the duty cycle is bigger (hence, the time the LED is on
is bigger).

Note: The timer PWM is mapped to the LED on port P2 using `PMAP_configurePorts`
in the `main`.
