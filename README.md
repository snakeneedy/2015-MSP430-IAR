# 2015 MSP430 IAR

## Macro & Sub-Runtine

- Macro 的實作為擴展程式碼，在 IAR 中 debug 較為困難
- Sub-Runtine 的 debug 容易許多，可以逐行執行，但 CALL 花的時間較多

### Macro
Macro 寫法:

    Labelname MACRO
    LOCAL Label1, Label2  ; 使外部不能 JMP 至此 label
    Label1:
        ; do something ...
    Label2:
        ; do something ...
        ENDM              ; end macro

如何使用:

    ; in main or sub program ...
        Labelname   ; 直接寫 macro 名稱即可

### Sub-Runtine
Sub-Runtine 寫法:

    Labelname:
        ; do something ...
        RET ; return

如何使用:

    ; in main or sub program ...
        CALL #Labelname   ; 使用 CALL 呼叫，記得加上 # 號

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

