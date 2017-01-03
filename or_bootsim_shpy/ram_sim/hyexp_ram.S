    
#define  LOOP_ON_ERROR  0
#define  TRAP_ON_ERROR  0


    .section .stack,"aw",@nobits
.space 0X10000
_stack:


    .section .text,"ax" 
    .org 0x100
    .global _start
_start:
    l.movhi r0,0
    l.ori r1,r0,1
    l.ori r2,r0,4    
    l.ori r3,r0,9
    l.ori r4,r0,16
    l.ori r5,r0,25
    l.ori r6,r0,36
    l.ori r7,r0,49
    l.ori r8,r0,64
    l.ori r9,r0,81
    l.ori r10,r0,100
    l.ori r11,r0,121
    l.ori r12,r0,144
    l.ori r13,r0,169
    l.ori r14,r0,196
    l.ori r15,r0,225
    l.sw  0(r0),r0
    l.sw  4(r0),r4
    l.sw  8(r0),r8
    l.ori r16,r0,1600
    l.ori r20,r0,2000
    l.sw 16(r0),r16
    l.sw 20(r0),r20
 
    l.movhi r1,hi(_stack-0)
    l.ori r1,r1,lo(_stack)

    l.movhi r4,0
    l.ori r4,r4,0x0100
    l.jr r4
    l.nop 0x0000
