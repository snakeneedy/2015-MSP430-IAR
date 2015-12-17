#include <msp430.h>

 void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P4SEL = BIT4+BIT5;                         // P4.4,5 = USCI_A1 TXD/RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |=  //???????????????????????//;    // CLK = SMCLK
//===========set the baud-rate register====
  UCA1BR0 = //???????????????????????//
  UCA1BR1 = //???????????????????????//
  UCA1MCTL = //???????????????????????//
//=========================================  
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, interrupts enabled
  __no_operation();                         // For debugger
}
// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA1IFG&UCTXIFG));             // USCI_A1 TX buffer ready?  
      //???????????????????????//           // TX -> RXed character
     break;	
  case 4:break;                             // Vector 4 - TXIFG
  default: break;  
  }
}
