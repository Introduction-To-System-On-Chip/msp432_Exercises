#include "ti/devices/msp432p4xx/inc/msp432p401r.h"
#include <stdint.h>

#define USE_IRQ 0

/* Handler redefinition for PORT1 GPIOs. */
void PORT1_IRQHandler(void)
{
  if (P1->IFG & BIT4)
  {
    P1->OUT ^= BIT0;
  }

  P1->IFG &= ~BIT4;
}


int main(void)
{
  /* Stop watchdog timer. */
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

  /* Configure clock. */
  CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_DIVA_0 |
             CS_CTL1_SELS_3 | CS_CTL1_DIVHS_0 | CS_CTL1_DIVS_0 |
             CS_CTL1_SELM_3 | CS_CTL1_DIVM_0;

  P1->OUT = 0;

  /* Set Port 1 Pin 0 (P1.0) direction to Output */
  P1->DIR |= BIT0;
  P1->OUT &= ~BIT0;

  /* Set Port 1 Pin 4 (P1.4) direction to Input */
  P1->DIR &= ~(BIT4);
  P1->OUT |= BIT4;
  /* Enable pull-up */
  P1->REN = BIT4;
  P1->SEL0 = 0;
  P1->SEL1 = 0;

  if (USE_IRQ)
  {
    /* Clear all interrupt flags. And enable them for button */
    P1->IFG = 0;
    /* High to low (i.e. when the button is pressed as there is a pull-up */
    P1->IES = BIT4;
    P1->IE = BIT4;


    // Enable Port 1 interrupt on the NVIC
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
    __enable_irq();

    /* Enter use deep sleep mode. */
    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);
    /* Go back to sleep after the IRQ is handled. */
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

  }
  else
  {
    /* Clear all interrupt flags. And make sure they are disabled.*/
    P1->IFG = 0;
    P1->IES = 0;
    P1->IE = 0;
  }

  /* Make sure all instructions are done. */
  __DSB();

  int ledActivate = 1;
  int pressed = 0;

  while(1)
  {
    if (USE_IRQ)
    {
      /* Go to sleep. */
      __sleep();
    }
    else
    {
      if (P1->IN & BIT4)
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
        P1->OUT |= BIT0;
      }
      else
      {
        P1->OUT &= ~BIT0;
      }
    }
  }

  return 0;
}
