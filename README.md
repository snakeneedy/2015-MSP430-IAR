# 2015 MSP430 IAR
Microprocessor Principles and Applications

# Lab Instruction Title

1. (none)
2. (none)
3. Instruction set and Instruction cycle
4. Modular Software Design
5. Mixing C and Assembly With the MSP430
6. Interrupts
7. Timer
8. Pulse Width Modulation (PWM)
9. UART, SPI
10. Analog-to-Digital Converter (A/D) Module


# IAR Basic
## Tool Bar
- Project
    - Create New Project...
        - asm -> asm (for Assembly)
        - c -> main (mix C and Assembly)
    - Add Existing Project...
- File
    - (about workspace: including all projects you have added in the IDE)

## Project Setting
(Right click the project in the Workspace)

- Option...
    - General Options
        - Device: `MSP430F5529` in this course
    - Debugger
        - Driver
            - `FET Debugger`: need device connected
            - `Simulator`: run without device

# MSP430 Architecture

- CPU has 16 registers
    - (R0/PC) Program Counter
    - (R1/SP) Stack Pointer
    - (R2/SR/CG1) Status Register
    - (R3/CG2) Constant Generator
    - (R4~R15) General Purpose
- SR bit description

<table>
    <tr>
        <td>15 - 19</td>
        <td>8</td>
        <td>7</td>
        <td>6</td>
        <td>5</td>
        <td>4</td>
        <td>3</td>
        <td>2</td>
        <td>1</td>
        <td>0</td>
    </tr>
    <tr>
        <td>Reserved</td>
        <td><code>V</code></td>
        <td>SCG1</td>
        <td>SCG0</td>
        <td>OSC OFF</td>
        <td>CPU OFF</td>
        <td>GIE</td>
        <td><code>N</code></td>
        <td><code>Z</code></td>
        <td><code>C</code></td>
    </tr>
</table>

- Reserved：The reserved bits are not used in the MSP430. 
- N：(Negative flag) Set if result is negative (MSB = 1), reset if positive (MSB = 0) 
- Z：(Zero flag) Set if result is zero, reset otherwise 
- C：(Carry bit) Set if there is a carry from the MSB of the result, reset otherwise 
- V：(Signed overflow flag) Set if the result of two positive operands is negative, or if the result of two negative numbers is positive, reset otherwise.


# ASM
## Basic code after project is created
### asm

    #include "msp430.h"                     ; #define controlled include file
            NAME    main                    ; module name

            PUBLIC  main                    ; make the main label vissible
                                            ; outside this module
            ORG     0FFFEh
            DC16    init                    ; set reset vector to 'init' label

            RSEG    CSTACK                  ; pre-declaration of segment
            RSEG    CODE                    ; place program in 'CODE' segment

    init:   MOV     #SFE(CSTACK), SP        ; set up stack

    main:   NOP                             ; main program
            MOV.W   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
            JMP $                           ; jump to current location '$'
                                            ; (endless loop)
            END

### c

    #include "io430.h"

    int main( void )
    {
      // Stop watchdog timer to prevent time out reset
      WDTCTL = WDTPW + WDTHOLD;

      return 0;
    }

## Syntax

- `; text`: 1 line comment
- `// text`: 1 line comment
- `/* ... */`: multiple line comment

### Arithmetic instructions

- `xxx.W` = `xxx`: **.W means word, 2 Bytes, 16 bits total.**
- `xxx.B`: **.B means 1 Byte, 8 bits total.**
- `MOV(.B) src, dst`  ; dst <- src
- `ADD(.B) src, dst`  ; dst <- dst + src
- `SUB(.B) src, dst`  ; dst <- dst - src
- `CMP(.B) src, dst`  ; dst - src, change the SR only
- `CLR(.B) dst`       ; dst <- 0, same as `MOV(.B) #0, dst`
- `INC(.B) dst`       ; dst <- dst + 1
- `DEC(.B) dst`       ; dst <- dst - 1

### Branch instructions

- `label_name: `   ; configure a label
- `xxx label_name` ; jump to label
- [Review] `CMP(.B) src, dst` ; dst - src
- `JMP` ; 無條件跳
- `JEQ/JZ` ; CMP兩數相等(Zero flag = 1) 跳
- `JNE/JNZ` ; CMP兩數不相等(Zero flag = 0) 跳
- `JGE` ; (Signed) dst >= src 時跳
- `JL` ; (Signed) dst < src 時跳
- `JN`: (dst - src) N Flag = 1 跳，結果是負的就跳，效果相當於 `JL`
- `JC`/`JHS` ; C Flag = 1 跳，代表有進位或者dst > src時跳
- `JNC`/`JLO`: C Flag = 0 跳，代表無進位或者dst < src時跳

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

## SP (Stack Pointer)

    SUB #Num1, SP
    ADD #Num2, SP
    PUSH #Num3
    ; SUB #2, SP
    ; MOV #Num3, 0(SP)
    POP R5
    ; MOV 0(SP), R5
    ; ADD #2, SP

<table>
    <tr>
        <td>SP:MAX</td>
    </tr>
    <tr>
        <td></td>
    </tr>
    <tr>
        <td></td>
    </tr>
    <tr>
        <td></td>
    </tr>
    <tr>
        <td>POP ↑</td>
    </tr>
    <tr>
        <td><b>SP</b></td>
    </tr>
    <tr>
        <td>PUSH ↓</td>
    </tr>
    <tr>
        <td></td>
    </tr>
    <tr>
        <td></td>
    </tr>
    <tr>
        <td></td>
    </tr>
    <tr>
        <td>SP:0</td>
    </tr>
</table>

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

