
//#include "io430.h"
#include  "msp430x552x.h"
#include "stdio.h"

int valuetemp=10;
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop Watch Dog Timer
  ADC12CTL0 = ADC12SHT0_8 + ADC12ON + ADC12MSC; // Set ADC12Memory0~7 sampling time(1/512Hz), turn on ADC12, enable sample and convert automatically.
  
  ADC12CTL1 =  ADC12CONSEQ0 + ADC12CONSEQ1 + ADC12SHP; // Set Repeat-sequence-of-channels(repeated autoscan), ADC sampling way.
  //ADC12CTL1 = ADC12CONSEQ1 + ADC12SHP;     // Set Repeat-single-channel, ADC sampling way.
  
  ADC12CTL1 |= ADC12SSEL1; // Set ADC tick from internal sampling timer.(as Default)
  
  
  ADC12CTL2 = ADC12RES_2;                   // Set 12bits resoultion(as Default)
  ADC12IE = ADC12IE0;                       // Enable interrupt
  ADC12MCTL0 = ADC12INCH0;                 // = set 0x01 as A1
  
  //ADC12MCTL1 = ADC12INCH1;                // = set 0x02 as A2
  ADC12MCTL5 = ADC12EOS;                    // Will stop scan in ADC memory05
  
  ADC12CTL0 |= ADC12ENC;                    // Conversion enabled
  
  // internal Pull-up/down register isn't working for non-IO using.
  // P6DIR &=~0x02;
  // P6REN |= 0x02; 
  // P6OUT &=~0x02; 
  
  P6SEL |= 0x02;                           // P6.1 ADC option select
  P4DIR |= 0x80;                           // P4.7 is output for LED
  P1DIR |= 0x01;                           // P1.0 is output for LED
  
  ADC12CTL0 |= ADC12SC;                   // Sampling open

   
  for (;;)
  {
    // This should actually happen in a timer interrupt where
    // we may like to sample only once in, say 1 second
     
      _BIS_SR(CPUOFF+GIE);                  // TURN OFF CPU, LEFT INTTERUPT PROCESS. SAVE ENERGY
  }
}

// ADC12 interrupt service routine
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR (void)
{
  switch(__even_in_range(ADC12IV,34))
  {
  case  0: break;                           // Vector  0:  No interrupt
  case  2: break;                           // Vector  2:  ADC overflow
  case  4: break;                           // Vector  4:  ADC timing overflow
  case  6:                                  // Vector  6:  ADC12IFG0 
   
    if (ADC12MEM0 > 0x800) {
      P4OUT |= 0x80; // LED P4.7 on                   // Set P6.2 - LED on
      P1OUT &= ~0x01; // LED P1.0 off
    }
    else {
      P4OUT &= ~0x80; // LED P4.7 off                      // Clear P6.2 - LED off
      P1OUT |= 0x01; // LED P1.0 on
    }
    
    valuetemp = ADC12MEM0;
//    printf("%d \n",valuetemp);
    
    _BIC_SR_IRQ(CPUOFF);                    // Clear CPUOFF bit from 0(SR)
    break;                           
    
  case  8: break;                           // Vector  8:  ADC12IFG1
  case 10: break;                           // Vector 10:  ADC12IFG2
  case 12: break;                           // Vector 12:  ADC12IFG3
  case 14: break;                           // Vector 14:  ADC12IFG4
  case 16: break;                           // Vector 16:  ADC12IFG5
  case 18: break;                           // Vector 18:  ADC12IFG6
  case 20: break;                           // Vector 20:  ADC12IFG7
  case 22: break;                           // Vector 22:  ADC12IFG8
  case 24: break;                           // Vector 24:  ADC12IFG9
  case 26: break;                           // Vector 26:  ADC12IFG10
  case 28: break;                           // Vector 28:  ADC12IFG11
  case 30: break;                           // Vector 30:  ADC12IFG12
  case 32: break;                           // Vector 32:  ADC12IFG13
  case 34: break;                           // Vector 34:  ADC12IFG14
  default: break;
  }
  
}