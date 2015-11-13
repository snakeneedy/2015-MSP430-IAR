#include "io430.h"
#include "intrinsics.h"

int sec10, sec47;

void delay();

void main(void)
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    P2DIR=0x00;
    P2SEL=0x00;
    P2REN=0x02;
    P2OUT=0x02;
    P2IE=0x02;
    P2IES=0x02;
    P2IFG=0x00;
    
    P1DIR=0x01;
    P1SEL=0x00;
    P1REN=0x02;
    P1OUT=0x02;
    P1IE=0x02;
    P1IES=0x02;
    P1IFG=0x00;
    
    //P1OUT=0x00;
    //P1DIR=0x01;
    P1SEL=0x00;
    
    P4OUT=0x00;
    P4DIR=0x80; // #1000 0000 b
    P4SEL=0x00;
    
    sec10 = sec47 = 0;
    
    __bis_SR_register(GIE);
    //GIE Interrupt Enable 
    for(;;)
    {
        if(sec10 == 0)
            P1OUT=0x02;
        else
            sec10--;
        if(sec47 == 0)
            P4OUT=0x00;
        else
            sec47--;
        delay();
    }

}

void delay()
{
    int i, i_max = 2250;
    for(i = 0; i < i_max; i++);
}

#pragma vector=PORT2_VECTOR
__interrupt void P2_ISR(void)
{
    P2IE=0x00;
    P1OUT=0x03;
    sec10 = 200;
    P2IFG=0x00;
    P2IE=0x02;
}

#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void)
{
    P1IE=0x00;
    P4OUT=0x80;
    sec47 = 200;
    P1IFG=0x00;
    P1IE=0x02;
}
