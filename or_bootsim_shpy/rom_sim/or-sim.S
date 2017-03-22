.section .text,"ax"

    .org 0x100
.global _start
_start:
    l.movhi r0,0
    l.movhi r1,0
    l.movhi r2,0
    l.movhi r3,0
    l.movhi r4,0
    l.movhi r5,0
    l.movhi r6,0
    l.movhi r7,0
    l.movhi r8,0
    l.movhi r9,0

    l.ori r5,r0,25
    l.ori r6,r0,36
    l.ori r7,r0,49
    l.ori r8,r0,64
    l.ori r9,r0,81

    l.sw 0(r0),r5
    l.sw 4(r0),r6
    l.sw 8(r0),r7
    l.sw 12(r0),r8
    l.sw 16(r0),r9
    l.nop
    l.lwz r20,0(r0)
    l.lwz r21,4(r0)
    l.lwz r22,8(r0)
    l.lwz r23,12(r0)
    l.lwz r24,16(r0)
#    l.lwz r15,0(r0)
#    l.lwz r17,8(r0)
#    l.lwz r16,4(r0)
#    l.lwz r19,16(r0)
#    l.lwz r18,12(r0)
    
    l.movhi r31,0x0000
    l.ori r31,r31,0x0100
    l.jr r31
    l.nop 
    l.nop 
