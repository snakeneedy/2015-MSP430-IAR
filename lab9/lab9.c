#include "msp430.h"
#include "stdio.h"
// Clock: 1 MHz
// Baud-rate: 9600 Hz
 void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P4SEL = BIT4+BIT5;                         // P4.4,5 = USCI_A1 TXD/RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |= UCSSEL_2;//?????//    // CLK = 10 SMCLK
//===========set the baud-rate register====
  // N = int( 2^{20}/9600 ) = 109
  UCA1BR0  = 109; //???????????????????????//     // low byte = 109
  UCA1BR1  =   0; //???????????????????????//     // high byte = 0
  UCA1MCTL =   4; //???????????????????????//     // 4: USBRS_2
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
//  printf("www\n");
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA1IFG&UCTXIFG));             // USCI_A1 TX buffer ready?  
      UCA1TXBUF = UCA1RXBUF;  //???????????????????????//      // TX -> RXed character
    break;	
  case 4:break;                             // Vector 4 - TXIFG
  default: break;  
  }
}
