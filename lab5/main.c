#include "io430.h"
int led_tra(void);
int DelayTenths(int A);
void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  P1OUT=0x00;
  P1DIR=0x01;
  while(1)
  {
    asm("push.w #20"); // Do something for loop
  DelayTenths(600);
  asm("add.w #2, SP"); //Back to original SP address
  led_tra();
  }
}
int led_tra(void)
{
    P1OUT^=0x01;
}
