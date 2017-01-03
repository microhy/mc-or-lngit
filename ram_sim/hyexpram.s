.section .text,"ax"

    .org 0x100
.global _start
_start:
    l.nop
    l.lwz r20,0(r0)
    l.lwz r21,4(r0)
    l.lwz r22,8(r0)
    l.lwz r23,12(r0)
    l.lwz r24,16(r0)
    l.lwz r15,0(r0)
    l.lwz r17,8(r0)
    l.lwz r16,4(r0)
    l.lwz r19,16(r0)
    l.lwz r18,12(r0)
    
    l.movhi r31,0xf000
    l.ori r31,r31,0x0100
    l.jr  r31
    l.nop 0x0000
    l.nop 0x0000
    l.nop 0x0000
    l.nop 0x0000
    l.nop 0x0000
    l.nop 0x0000
    l.nop 0x0000
    l.nop 0x0000
