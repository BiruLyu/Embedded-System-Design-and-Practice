/*********************************************************************************************
* File£º	led.c
* Author:	embest
* Desc£º	control board's LEDs
* History:	
*********************************************************************************************/
#include "44b.h"
#include "44blib.h"

/*------------------------------------------------------------------------------------------*/
/*                                     global variables                                     */
/*------------------------------------------------------------------------------------------*/
int f_nLedState;                                                // LED status   
            
/*------------------------------------------------------------------------------------------*/
/*                                     function declare                                     */
/*------------------------------------------------------------------------------------------*/
void led_test();                                                // LED test	
void leds_on();                                                 // all leds on  
void leds_off();                                                // all leds off
void led1_on();                                                 // led 1 on 
void led1_off();                                                // led 1 off 	
void led2_on();                                                 // led 2 on 
void led2_off();                                                // led 2 off 
void led3_on();                                                 // led 3 on 
void led3_off();                                                // led 3 off 	
void led4_on();                                                 // led 4 on 
void led4_off();                                                // led 4 off 
void led_display(int nLedStatus);                               // led control 

/*********************************************************************************************
* name:		led_test
* func:		leds test funciton
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void led_test()
{
	
	leds_off();
	
	delay(3000);

	// 1 on -> 2 on -> all on -> 2 off -> 1 off
	led1_on();
	delay(3000);
	led1_off();
	delay(3000);
	led3_on();
	delay(3000);
	led1_on();
	delay(3000);
	beep(0);
	led3_off();
	delay(3000);
	led1_off();
	
}

/*********************************************************************************************
* name:		leds_on,led_off
* func:		all leds light or close 
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void leds_on()
{
	led_display(0xF);
}

void leds_off()
{
	led_display(0x0);
}

/*********************************************************************************************
* name:		led1_on,led1_off
* func:		led 1 light or close 
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void led2_on()
{
	f_nLedState = f_nLedState | 0x1;
	led_display(f_nLedState);
}

void led2_off()
{
	f_nLedState = f_nLedState&0xfe;
	led_display(f_nLedState);
}

/*********************************************************************************************
* name:		led2_on,led2_off
* func:		led 2 light or close  
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void led4_on()
{
	f_nLedState = f_nLedState | 0x2;
	led_display(f_nLedState);
}

void led4_off()
{
	f_nLedState = f_nLedState&0xfd;
	led_display(f_nLedState);
}

/*********************************************************************************************
* name:		led3_on,led3_off
* func:		led 3 light or close  
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void led1_on()
{
	f_nLedState = f_nLedState | 0x4;
	led_display(f_nLedState);
}

void led1_off()
{
	f_nLedState = f_nLedState&0xFB;
	led_display(f_nLedState);
}

/*********************************************************************************************
* name:		led4_on,led4_off
* func:		led 4 light or close  
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void led3_on()
{
	f_nLedState = f_nLedState | 0x8;
	led_display(f_nLedState);
}

void led3_off()
{
	f_nLedState = f_nLedState&0xf7;
	led_display(f_nLedState);
}

/*********************************************************************************************
* name:		led_display
* func:		light or close the Led 1,2 
* para:		nLedStatus	-- input, light LED 1,2 according to the nLedStatus' bit[2:1]
*								  nLedStatus' bit[2:1] = 00 : LED 2,1 off
*								  nLedStatus' bit[2:1] = 11 : LED 2,1 on
*								  nLedStatus   = 1 : LED 1 on
*								  nLedStatus   = 2 : LED 2 on
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void led_display(int nLedStatus)
{
	f_nLedState = nLedStatus;
	
	// change the led's current status
	if((nLedStatus&0x01) == 0x01)
		rPDATC &= 0xFEFF;                                 // GPC8:LED1 (D1204) on
	else
		rPDATC |= (1<<8);                                 // off

	if((nLedStatus&0x02) == 0x02)
		rPDATC &= 0xFDFF;                                 // GPC9:LED2 (D1205) on
	else
		rPDATC |= (1<<9);                                 // off

	if((nLedStatus&0x04) == 0x04)
		rPDATF &= 0xEF;                                   // GPF4:LED3 (D1206) on
	else
		rPDATF |= (1<<4);                                 // off

	if((nLedStatus&0x08) == 0x08)
		rPDATF &= 0xF7;                                   // GPF3:LED4 (D1207) on
	else
		rPDATF |= (1<<3);                                 // off
}
