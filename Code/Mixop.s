/*------------------------------------------------------------------------------------------*/
/*	 								global symbol define						 			*/
/*------------------------------------------------------------------------------------------*/
	.global mixop
	.extern multiply	
/*------------------------------------------------------------------------------------------*/
/*	 								code								 				    */
/*------------------------------------------------------------------------------------------*/	
	.text
mixop:
	ldr	sp, =0x00002000
 	mov	   ip, sp
	stmdb  sp!, {fp, ip, lr, pc}
	sub	   fp, ip, #4	
	stmdb  sp!, {r0,r1,r2}  #压栈保存r0-r2
	mov    r0,r1    #准备函数multiply的两个参数
	mov    r1,r2
    bl     	multiply
    mov     r3,r0   #将函数multiply的返回结果保存在r3
    ldmia   sp!, {r0,r1,r2}  #从栈中取出r0-r2
    add     r0,r0,r3  #计算加法
	ldmdb   fp, {fp, sp, pc}
    .end
