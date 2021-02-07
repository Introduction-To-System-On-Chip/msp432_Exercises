# IRQ versus Polling

## Goals

- Understand the difference between IRQ and Polling.
- Use a button to switch on and off an LED when pressed.
- Look at the registers in the Technical Reference Manual (TRM) to configure the
  peripheral and behavior of the device.


# Instructions

A copy of the TRM can be found in `../doc/slau356i.pdf` to get the register
descriptions.

Open the main.c and fill in the TODOs in the code to:

- Configure the clock by writing into the Clock System (CS) registers: KEY,
  CTL0 and CTL1. All fields are described in the Technical Reference Manual.

- Configure the button as input and LED as output. Use P1->DIR, P1->OUT, P1->SEL
  registers.

- Start with USE_IRQ == 0 and disable all interrupts.

- Fill the `setLed()` and `isButtonPressed()` functions to set the LED state
  and get the button status. Use the P1->IN and P1->OUT registers.

When launched, the program should blink the LED when the button is pressed.

To use the IRQ mode, update the define to `#define USE_IRQ 1` and fill the TODOs
to:

- Fill the `blinkLed()` function to invert the state of the LED (if the LED is
  OFF, set it ON and vice versa). Use P1->OUT register.

- Inside the `PORT1_IRQHandler()` to get the interrupt number, blink the LED
  using `blinkLed()` and ack the IRQ. Use P1->IFG register.

- In the `main()`, enable interrupts for the button, P1 port and overall. Use
  P1->IFG, P1->IES, P1->IE, NVIC->ISER. The button interrupt should be triggered
  from High to Low.

- Use the SCB->SCR to directly enter in deepsleep mode after an IRQ is served.

Note: For the IRQ mode, the `__sleep()` function is used. This is to make the
device enter in low power. If the proper setting is used, IRQ will wake it up.
For the polling mode, there is no sleep and delay as the software has to
periodically look at the status of the button to act on the LED state.

- Use Energy trace to look at the power consumption and see the difference
  between IRQ and Polling.
