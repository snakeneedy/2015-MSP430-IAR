#include "io430.h"
#include "intrinsics.h"
#include "math.h"
#include "stdio.h"
// 3-1

#define FREQ_A4 440.0
#define FREQ_B4 493.9
#define FREQ_C4 523.3
#define FREQ_C4_UP 554.4
#define FREQ_D4 587.3
#define FREQ_E4 659.3

#define FREQ_C5 523.3
#define FREQ_D5 587.3
#define FREQ_E5 659.3
#define FREQ_F5 698.5
#define FREQ_G5 784.0
#define FREQ_A5 880.0
#define FREQ_B5 987.8

#define F0 1048576.0

int index, index_max;
int is_gap, is_on;
//int 
char num_ch[] = "115566544332215544332554433211556654433221";
char time[]   = "444444244444424444442444444244444424444442";
void trigger_pause(void);

void main(void)
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    // P1.0 out, debug
    // P1.1 in
    // P1.2 out
    // P1.4 in
    // P1.5 in
    P1DIR=0x34; // #00000100b
//    P1DIR=0x35; // #00110101b, debug
    P1SEL=0x00; // #00000000b
    P1REN=0x02; // #00110010b
    P1OUT=0x02; // #00000010b
    P1IE =0x32; // #00110010b, BIT5|BIT4|BIT1
    P1IES=0x02; // #00110010b
    P1IFG=0x00; // #00000000b
    P1SEL=0x04; // #00110100b, P1SEL.2
    // P2.1 in
    P2DIR=0x00;
    P2SEL=0x00;
    P2REN=0x02;
    P2OUT=0x02;
    P2IE =0x02;
    P2IES=0x02;
    P2IFG=0x00;
    // P7.4 out
//    P7DIR=0x10;
//    P7SEL=0x10;
    // P1.2 timer: TA0.1
    TA0CTL=TASSEL_2+MC_1+ID_0+TACLR;
    TA0CCTL1=OUTMOD_3;
    TA0CCR0=0;
    TA0CCR1=0;
    
    index = is_gap = is_on = 0;
    index_max = 41;
    // P7.4 timer: TB0.2
//    TB0CTL=TASSEL_2+MC_1+ID_0+TBCLR;
//    TB0CCR0=3823;
//    TB0CCR2=1136;
//    TB0CCTL2=OUTMOD_3;
    // timer: TA1.0
    TB0CTL=TASSEL_1+MC_1+ID_0+TBCLR;
//    TB0CCTL0=CCIE;
    TB0CCR0=16384; // 0.5 sec
    // trigger_pause();
	
	
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
	
	
    __bis_SR_register(GIE+CPUOFF);  // GIE Interrupt Enable & CPU off
}

void update_voice(void)
{
    switch(num_ch[index]) {
    case '1':
        TA0CCR0=(int)round(F0/FREQ_C5);
        break;
    case '2':
        TA0CCR0=(int)round(F0/FREQ_D5);
        break;
    case '3':
        TA0CCR0=(int)round(F0/FREQ_E5);
        break;
    case '4':
        TA0CCR0=(int)round(F0/FREQ_F5);
        break;
    case '5':
        TA0CCR0=(int)round(F0/FREQ_G5);
        break;
    case '6':
        TA0CCR0=(int)round(F0/FREQ_A5);
        break;
    case '7':
        TA0CCR0=(int)round(F0/FREQ_B5);
        break;
    default:
        TA0CCR0=0;
        break;
    }
    TA0CCR1 = (int)(TA0CCR0 * 0.125);
}

void set_voice_freq(double freq) {
	TA0CCR0 = (int)round(F0/freq);
	TA0CCR1 = (int)(TA0CCR0 * 0.125);
}
void set_voice_mute() {
	TA0CCR0 = TA0CCR1 = 0;
}

void trigger_pause(void)
{
	// pause & continue
    TB0CCTL0 ^= CCIE;
    is_on = 1 - is_on;
    if(is_on==1) {// failed
        TB0CTL|=MC_1;
        //0x0100 (256,0)
    }
    else {
        TA0CCR0 = 0;
        TA0CCR1 = 0;
        TB0CTL&=~MC_1;
        //0x0111 (272,0)
    }
}

void trigger_stop(void)
{
    index = is_gap = is_on = 0;
    TA0CCR0 = 0;
    TA0CCR1 = 0;
    TB0CCR0=16384; // 0.5 sec
    TB0CCTL0 &= ~CCIE;
}

#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void)
{
    P1IE=0x00;
    //
	if( P1IFG & BIT4 ) {
		// P1.4
		trigger_pause();
	}
	else {
		// P1.1, P1.5
		trigger_stop();
	}
    //
    P1IFG=0x00;
    P1IE =0x32; // #00110010b, BIT5|BIT4|BIT1
    __bis_SR_register(GIE+CPUOFF);  // GIE Interrupt Enable & CPU off
}

#pragma vector=PORT2_VECTOR
__interrupt void P2_ISR(void)
{
    // start/pause/continue
    P2IE=0x00;
    //
	trigger_pause();
    //
    P2IFG=0x00;
    P2IE=0x02;
    __bis_SR_register(GIE+CPUOFF);  // GIE Interrupt Enable & CPU off
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void)
{
//    P1OUT^=0x01; // debug
    if(is_gap==0) {
        if(time[index]=='4') {
            TB0CCR0 = 16384;
        } else if(time[index]=='2') {
            TB0CCR0 = 32767;
        }
        update_voice();
        if(index <= index_max)
            index++;
        else
            P2IFG&=0x01;
    }
    else {
        TA0CCR0 = 0;
        TA0CCR1 = 0;
        TB0CCR0 = 1638; // 0.05 sec
    }
    is_gap = 1 - is_gap;
    __bis_SR_register(GIE+CPUOFF);  // GIE Interrupt Enable & CPU off
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
//  printf("www\n");
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA1IFG&UCTXIFG));             // USCI_A1 TX buffer ready?
	
	// UCA1TXBUF = UCA1RXBUF;  //???????????????????????//      // TX -> RXed character
	// printf("%d, %d\n", UCA1TXBUF, UCA1RXBUF);
	//trigger_pause();
	switch (UCA1RXBUF) {
	case '1':
		set_voice_freq(FREQ_A4);
		break;
	case '2':
		set_voice_freq(FREQ_B4);
		break;
	case '3':
		set_voice_freq(FREQ_C4);
		break;
	case '4':
		set_voice_freq(FREQ_C4_UP);
		break;
	case '5':
		set_voice_freq(FREQ_D4);
		break;
	case '6':
		set_voice_freq(FREQ_E4);
		break;
// #define FREQ_A4 440.0
// #define FREQ_B4 493.9
// #define FREQ_C4 523.3
// #define FREQ_C4_UP 554.4
// #define FREQ_D4 587.3
// #define FREQ_E4 659.3
	case 'z':
		set_voice_freq(FREQ_B4);
		break;
	}
    break;	
  case 4:break;                             // Vector 4 - TXIFG
  default: break;  
  }
}

