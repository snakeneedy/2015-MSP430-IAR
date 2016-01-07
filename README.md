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

## Mixing multiple assembly codes

- in which you want to include:
    - `PUBLIC` the label name
    - implement what label do
    - ex. [lab-4.0.s43](https://github.com/snakeneedy/2015-MSP430-IAR/blob/master/lab4/lab-4.0.s43#L2)
- the one want to include others
    - `EXTERN` the label name
    - `CALL #label_name`
    - ex. [lab-4.1.s43](https://github.com/snakeneedy/2015-MSP430-IAR/blob/master/lab4/lab-4.1.s43#L19)
- **Notice**: the registers used in different codes will conflict, make sure not using the same registers before including.

## include ASM into C

- First you should create two (or more) different types of projects
    - some for ASM and some for C
- in ASM
    - `PUBLIC` the label name
        - ex. `PUBLIC mpy`
    - implement what label do
    - **DEBUG the ASM code** in IAR IDE
- in C
    - `extern` the prototype for ASM
        - such as `extern int mpy (short arge1, short arge2);`
    - Right-click the project created, then choose **Add** -> **Add Files...**
    - Choose the file: `{project-of-ASM}/Debug/Obj/asm.r43`

### Passing parameters from C into ASM

**(not yet)**

# Interrupts
## Basic Instruction
- Software Interrupts
    - exceptional condition
        - ex. Divided by zero, System calls, ... etc
    - special instruction
        - from Device drivers
- Wardware Interrupts
    - ex. Pressing key, ... etc
- When the interrupt occurs, the processor ...
    1. Stop what it was doing.
    2. Save enough information (PC & SR) for it to resume later on.
    3. Identify the source of the interrupt.
    4. Execute an interrupt service routine (ISR).
    5. Restore PC & SR from stack.
    6. Resume the interrupted program when the ISR has been completed.
- Maskable and Non-maskable
    - Maskable: clearing the General Interrupt Enable (GIE) in SR
    - Non-maskable:
        - System Reset : Power up, External Reset...
        - System NMI : PWM, JTAG Mailbox...
        - User NMI : Oscillator fault...

## Register

`Pn???`, n <- port number

<table>
  <tr><td>Register</td><td>Description</td><td>Type</td><td>Initial Value</td><td>暫存器名稱</td></tr>
  <tr><td><pre>P2IN</pre></td><td>Port P2 Input</td><td>R only</td><td>-----</td><td>輸入暫存器</td></tr>
  <tr><td><pre>P2OUT</pre></td><td>Port P2 Output</td><td>R / W</td><td>No Change</td><td>輸出暫存器</td></tr>
  <tr><td><pre>P2DIR</pre></td><td>Port P2 Direction</td><td>R / W</td><td>00000000b</td><td>方向暫存器</td></tr>
  <tr><td><pre>P2IFG</pre></td><td>Port P2 Interrupt Flag</td><td>R / W</td><td>00000000b</td><td>中斷旗號暫存器</td></tr>
  <tr><td><pre>P2IE</pre></td><td>Port P2 Interrupt Enable</td><td>R / W</td><td>00000000b</td><td>中斷致能暫存器</td></tr>
  <tr><td><pre>P2IES</pre></td><td>Port P2 Interrupt Edge Select</td><td>R / W</td><td>No Change</td><td>中斷邊緣選擇暫存器</td></tr>
  <tr><td><pre>P2SEL</pre></td><td>Port P2 Selection</td><td>R / W</td><td>00000000b</td><td>功能選擇暫存器</td></tr>
</table>

## EINT, DINT

- Enable Interrupt
	- `ENIT`
	- `GIE <- 1`
	- `BIS.W  #8, SR`
- Disable Interrupt
	- `DINT`
	- `GIE <- 0`
	- `BIC.W  #8, SR`

## in ASM
Syntax:

	ORG    word_address
	DW     label_name

Take button `P2.1` for example

	// before "init"
	    ORG    0FFD4h             // I/O Port P2 Word Address
	    DW     P2_ISR
	
	init:
	    BIC.B  #00000010b, &P2DIR // direction: in
	    BIC.B  #00000010b, &P2IFG // interrupt flag
	    BIC.B  #00000010b, &P2SEL // selection (function)

	    BIS.B  #00000010b, &P2REN // pullup or pulldown enable
	    BIS.B  #00000010b, &P2OUT
	    BIS.B  #00000010b, &P2IE  // interrupt enable
	    BIS.B  #00000010b, &P2IES // interrupt edge select
	    EINT                      // enbale interrupt
	    // ... do something
	    BIS.W  #CPUOFF, SR        // cpu off
	    JMP $
	
	P2_ISR:
	    BIC.B  #00000010b, &P2IE
	    // ... do something
	    BIC.B  #00000010b, &P2IFG
	    BIS.B  #00000010b, &P2IE
	    RETI // return interrupt
	
	    END


## in C
Syntax

	// in main
	//     setting the interrupt to the corresponding port
	// outside main
	#pragma vector=XXX_VECTOR // check "io430f5521.h" for MSP430F5529
	__interrupt void isr_name(void)
	{
	    // ... do something
	}

io430f5521.h: [https://github.com/snakeneedy/2015-MSP430-IAR/blob/master/doc/io430f5521.h](https://github.com/snakeneedy/2015-MSP430-IAR/blob/master/doc/io430f5521.h)

Take button `P2.1` for example

	#include "io430.h"
	void main(void)
	{
	    WDTCTL = WDTPW+WDTHOLD;
	
	    P2DIR &= ~0x02; // same as P2DIR &= ~BIT1;
	    P2IFG &= ~0x02;
	    P2SEL &= ~0x02;
	    P2REN |= 0x02;
	    P2OUT |= 0x02;
	    P2IE  |= 0x02;
	    P2IES |= 0x02;
	
	    // interrupt enable + cpuoff
	    __bis_SR_register(GIE+CPUOFF);
	
	    //return; // disable
	}
	
	#pragma vector=PORT2_VECTOR
	__interrupt void P2_ISR(void)
	{
	    P2IE  &= ~0x02; // disable interrupt
	    // ... do something
	    P2IFG &= ~0x02; // clean interrupt flag
	    P2IE  |= 0x02;  // re-enable interrupt
	}

## Interrupt Table
Interrupt Vector Address: [https://github.com/snakeneedy/2015-MSP430-IAR/blob/master/doc/Interrupt-Vector-Addresses.pdf](https://github.com/snakeneedy/2015-MSP430-IAR/blob/master/doc/Interrupt-Vector-Addresses.pdf)


# Timer
## Clock Source
- ACLK 32768(Hz)
- MCLK 1M
- SMCLK 1M

## Watchdog Timer Control Register (`WDTCTL`)
## Timer A Control Register (`TAxCTL`)
## Capture/Compare Control Register (`TAxCCTLn`)

# Other
## 32-bit multiplier
(32-bit)x(32-bit)=(64-bit)

Sample code: [http://relic-toolkit.googlecode.com/svn/trunk/src/low/msp-asm/fp\_sqr32\_256\_comba.inc](http://relic-toolkit.googlecode.com/svn/trunk/src/low/msp-asm/fp_sqr32_256_comba.inc)

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

## Appendix

- `__bcd_add_short`
- `__bcd_add_long`
- `__bcd_add_long_long`
- `__bis_SR_register`
- `__bis_SR_register_on_exit`
- `__bic_SR_register`
- `__bic_SR_register_on_exit`
- `__enable_interrupt`
- `__disable_interrupt`
- `__even_in_range`
- `__get_interrupt_state`
- `__get_R4_register`
- `__get_R5_register`
- `__get_SP_register`
- `__get_SR_register`
- `__get_SR_register_on_exit`
- `__low_power_mode_n` (n: 0~4)
- `__low_power_mode_off_on_exit`
- `__no_operation`
- `__op_code`
- `__segment_begin`
- `__segment_end`
- `__set_interrupt_state`
- `__set_R4_register`
- `__set_R5_register`
- `__set_SP_register`

