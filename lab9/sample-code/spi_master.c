
#include "io430.h"

unsigned char temp;

void main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  
  //button setting
  P2DIR &=  ~BIT1;
  P2OUT = BIT1;
  P2REN = BIT1;
  P2IES = BIT1;
  P2IE  = BIT1;
   
  P1DIR |= 0x01;                           // Set P1.0 to output direction

  P3SEL |= BIT3+BIT4;                       // P3.3,4 option select
  P2SEL |= BIT7;                            // P2.7 option select
  
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    // 3-pin, 8-bit SPI master
  UCA0CTL0 &= ~(BIT2+BIT1);                 // 3-pin, 8-bit SPI master
                                            // Clock polarity high, MSB
  UCA0CTL1 |= UCSSEL_1;                     // ACLK
  UCA0BR0 = 0x02;                           // /2
  UCA0BR1 = 0;                              //
  UCA0MCTL = 0;                             // No modulation
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
  UCA0TXBUF = 0;  

   __bis_SR_register(LPM0_bits + GIE);        // CPU off, enable interrupts
   __no_operation(); 
  
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
  __bic_SR_register(GIE); 
  UCA0TXBUF = 1;  
  P2IFG &= ~BIT1;
  return ;
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  volatile unsigned int i;
  __bic_SR_register(GIE); 
  switch(__even_in_range(UCA0IV,4))
  {
    case 0: break;                          // Vector 0 - no interrupt
    case 2:                                 // Vector 2 - RXIFG
      UCA0IFG = ~UCRXIFG;  
      break;
    case 4: break;                          // Vector 4 - TXIFG
    default: break;
    
  }
  return ;
}



