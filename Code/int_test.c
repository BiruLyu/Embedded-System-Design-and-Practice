#include	"44blib.h"
#include	"44b.h"
#include	"def.h"

/*------------------------------------------------------------------------------------------*/
/*                                     function declare                                     */
/*------------------------------------------------------------------------------------------*/
void init_int(void);
void int_test(void);
void int4567_isr(void);// __attribute__ ((interrupt ("IRQ")));

/*------------------------------------------------------------------------------------------*/
/*                                     global variables                                     */
/*------------------------------------------------------------------------------------------*/
unsigned char f_ucIntNesting = 0;			// Interrupt nesting count
unsigned char f_ucWhichInt   = 0;			// interrupt source symbol

/*********************************************************************************************
* name:		init_int
* func:		initialize the extern interrupt control
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void init_int(void)
{
	// interrupt settings
	rI_ISPC    = 0x3ffffff;					// clear interrupt pending register
	rEXTINTPND = 0xf;						// clear EXTINTPND register
	rINTMOD    = 0x0;						// all for IRQ mode
	rINTCON    = 0x5;						// nonVectored mode, IRQ disable, FIQ disable    
	rINTMSK    = ~(BIT_GLOBAL|BIT_EINT4567);
		
	// set EINT interrupt handler
	pISR_EINT4567 = (int)int4567_isr;
    
	// PORT G configuration
	rPCONG  = 0xffff;						// EINT7~0
	rPUPG   = 0x0;							// pull up enable	    
	rEXTINT = rEXTINT | 0x22220020;			// EINT4567 falling edge mode	
	rI_ISPC |=  BIT_EINT4567;
	rEXTINTPND = 0xf;						// clear EXTINTPND reg
}

/*********************************************************************************************
* name:		int_test
* func:	
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void int_test(void)
{
	unsigned int unSaveG,unSavePG;
        int tag;
	//init_int();
	rINTMSK = rINTMSK | BIT_EINT4567;		// disable EINT2 int
	// user interface
	uart_printf("\n\r实验五 中断\n");   
	/*uart_printf(//" Please Select the trigger:\n"
	            "Select 1-9 to determine the Flashing frequency\n"
				"  any key to exit...\n");*/				
        uart_printf("请输入一个1~9的数字：\n");
	// save the current settings of Port G controler
	unSaveG = rPCONG;
	unSavePG= rPUPG;
	rPCONG  = 0xf5ff;						// EINT7~0
	rPUPG   = 0x0;							// pull up enable
	switch(uart_getch())
	{
		case '1':
			tag=1;			
			break;	    
		case '2':
			tag=2;			
			break;
		case '3':
		    tag=3;
			break;
		case '4':
		    tag=4;
			break;
		case '5':
			tag=5;			
			break;	    
		case '6':
			tag=6;			
			break;
		case '7':
		    tag=7;
			break;
		case '8':
		    tag=8;
			break;
		default:
		    tag=9;
			return;
	}

	//uart_printf(" Press the buttons \n");
        uart_printf("按下按钮！\n");
	uart_printf(" push buttons may have glitch noise problem \n");
	rINTMSK = ~(BIT_GLOBAL | BIT_EINT4567);  
	while(!f_ucWhichInt);					// waiting for the interrupt
	f_ucIntNesting = 1;						// Allow to Nesting.

	switch(f_ucWhichInt)
	 {
		case 1:
			uart_printf(" EINT4 had been occured... \n");
			break;
		    
		case 2:
			uart_printf(" EINT5 had been occured...\n");
			break;
		    
		case 4:								// SB2 --- flash LED1
			uart_printf(" EINT6 had been occured... LED1 (D1204) on\n");
			
			// flesh LED1
			leds_off();
			while(tag--)
			{
			led1_on();
			delay(10000);					// 10000 x 100 us
			led1_off();
			delay(10000);
			}
			break;
		    
		case 8:								// SB3 --- flash LED2
			uart_printf(" EINT7 had been occured... LED2 (D1205) on\n");
			leds_off();
			// flesh LED2
			while(tag--)
			{
			led4_on();
			delay(10000);					// 10000 x 100 us
			led4_off();
			delay(10000);
			}
			break;
		    
		case 9:
			uart_printf(" The extern interrupt had been occured (1 level mode)\n");
		    break;
		    
		default :
			uart_printf(" Error!\n");
			break;
	  }
