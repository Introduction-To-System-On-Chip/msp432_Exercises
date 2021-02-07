# Timers

## Goal

- Understand the basic concepts behind Clock and Timer configuration.
- Create a function to compute time elapsed in seconds and milliseconds.
- Compare power consumption of different clocks.
- Look at the registers in debug mode

# Instructions

- In `initTimer` setup a timer to count the number of second and millisecond
  elapsed. Be careful about the clock source selection.

- Update the `getTime` function to compute the seconds and miliseconds elapsed
  from the current counter value of the timer. Fill the `currentTime_t`
  structure.

- Configure the MCLK as DCO (1.5MHz), REFO, MODO. Adjust the divider and look at
  the time difference to finish the program. Putty or Teraterm or Minicom can
  be used to see the printed values form the board. Use the 9600 baudrate. Using
  energy trace, it is also possible to monitor this time as the current will
  increase during the processing of the loop.
  The button can be configured to restart the loop again.

- Use energy trace monitor to look at the power consumption if the MCLK source
  is changed. Have a look at `../doc/msp432p401r.pdf` which contains the power
  consumption of the MSP432 clocks.

## Questions

- With the current timer, what is the maximal time (in seconds, miliseconds) it
  can count to before restarting to 0? What happens if a divider is used?

- Can you create an interrupt each 1 milliseconds using a timer with a 32.768kHz
  input frequency?

- Does changing the divider of the MCLK impact the overall consumption?
