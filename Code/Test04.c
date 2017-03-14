#include "rw_ram.h"
extern void s_init(int src_addr, int num);
extern void c_move(int src_addr, int num, int dst_addr);

void    mem_test(void);
void    Test04(void);

void Test04(void)
{

    //int src_addr=0xc100002, num=15, dst_addr=0xc200017;
    
    //sys_init();        /* Initial 44B0X's Interrupt,Port and UART */

    // user interface 
    uart_printf("\n\rÊµÑéËÄ ´æ´¢Æ÷\n");
    uart_printf(" Memory read  base Address : 0x%x\n",RW_BASE); 
    s_init(RW_BASE,RW_NUM);
    uart_printf(" Memory write base Address : 0x%x\n",RW_TARGET); 
    uart_printf("\n Memory Read/Write (ASM code,%dBytes) Test. \n",RW_NUM); 
    c_move(RW_BASE,RW_NUM,RW_TARGET);
    uart_printf("\n Memory Read/Write (C code,%dBytes) Test. \n",RW_NUM); 
    uart_printf(" Memory Test Success! \n"); 
//    test_cache();

    //while(1);
}         

void mem_test(void)
{
    uart_printf(" Memory read  base Address : 0x%x\n",RW_BASE); 
    uart_printf(" Memory write base Address : 0x%x\n",RW_TARGET); 
    uart_printf("\n Memory Read/Write (ASM code,%dBytes) Test. \n",RW_NUM); 
   

    uart_printf("\n Memory Read/Write (C code,%dBytes) Test. \n",RW_NUM); 
    //c_ram_test();
    
    uart_printf(" Memory Test Success! \n"); 
}
