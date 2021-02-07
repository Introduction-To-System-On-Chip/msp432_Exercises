#include "ti/devices/msp432p4xx/inc/msp432p401r.h"
#include <stdbool.h>

// When 0, uses the Polling mode, when 1 use the IRQ
#define USE_IRQ 0

static bool isButtonPressed(void)
{
  /* TODO: Check if the Button Pin P1.4 is set */
  return false;
}

static void blinkLed(void)
{
  /* TODO: Blink the LED pin P1.0 */
}

static void setLed(bool enable)
{
  /* TODO: Set the LED pin P1.0 depending on function argument */
}

/* Handler redefinition for PORT1 GPIOs. */
void PORT1_IRQHandler(void)
{
  /* TODO: Make sure the IRQ comes from the P1.4 Button */
  

  /* TODO: Clear the corresponding interrupt from P1.4 */
}

int main(void)
{
  /* Hold watchdog timer. */
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

  /*
   * TODO: Configure clock:
   * - Unlock CS register
   * - Set DCO to 3MHz
   * - Set ACLK to REFO, SMCLK to DCO, MCLK to DCO
   */

  /* TODO: Set LED P1.0 direction to Output */

  /* TODO: Set Button P1.4 direction to Input */

  /* TODO: Enable pull-up for P1.4 */

  /* TODO: Clear all interrupt flags */

#if USE_IRQ == 1
  /* TODO: Enable interrupt when the button is pressed */

  /* TODO: Enable the P1 PORT in the NVIC */

  /* 
   * TODO: Configure the device to use the DeepSleep mode and
   * to sleep after handling an IRQ
   */

  /* TODO: Enable all interrupts */

#else
  /* TODO: Disable all interrupts */

  int ledActivate = 1;
  int pressed = 0;
#endif

  /* Make sure all instructions are done. */
  __DSB();

  while(1)
  {

#if USE_IRQ == 1
    /* Go to sleep. */
    __sleep();
    
    /* This code will be reached only after an IRQ */

#else
    if (isButtonPressed())
    {
      pressed = 1;
    }
    else
    {
      if (pressed)
      {
        ledActivate = !ledActivate;
        pressed = 0;
      }
    }

    if (ledActivate)
    {
      setLed(true);
    }
    else
    {
      setLed(false);
    }
#endif

  }

  return 0;
}
