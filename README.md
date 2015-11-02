# 2015 MSP430 IAR

## 32-bit multiplier
(32-bit)x(32-bit)=(64-bit)

Sample code: [http://relic-toolkit.googlecode.com/svn/trunk/src/low/msp-asm/fp_sqr32_256_comba.inc](http://relic-toolkit.googlecode.com/svn/trunk/src/low/msp-asm/fp_sqr32_256_comba.inc)

    Multiplier:
        MOV #Num1, &MPY32L    ; Unsigned
        MOV #Num2, &MPY32H
        ;MOV #Num1, &MPYS32L  ; Signed
        ;MOV #Num2, &MPYS32H
        MOV #Num3, &OP2L
        MOV #Num4, &OP2H
        ;...
        MOV &RES0, R4
        MOV &RES1, R5
        MOV &RES2, R6
        MOV &RES3, R7

