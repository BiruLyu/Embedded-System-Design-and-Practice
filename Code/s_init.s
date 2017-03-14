.global s_init


s_init:
    ldr		sp,=0x2000
	stmdb	sp!, {r2-r4, lr}      
	@ read from RW_BASE and write them into RW_TARGET
    ldrb	r3,=0x1

loop:
	strb	r3,[r0],#1
	add		r3,r3,#1
	sub		r1,r1,#1
	cmp		r1,#0
	bne		loop
	
	ldmia	sp!, {r2-r4, lr}           
	mov	    pc,lr                      /* return */

