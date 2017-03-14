#include "44b.h"
#include "44blib.h"
#include "def.h"
#include "iic.h"

/*------------------------------------------------------------------------------------------*/
/*	 								global variables					 				    */
/*------------------------------------------------------------------------------------------*/
int f_nKeyPress;

unsigned char f_szDigital2[] ={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,// 0 ~ 9
                              0xEE,0xFF,0x9C,0xFD,0x9E,0x8E,// A 8. C 0. E F
                              0x7A,0xB7,0x7C,0x3E,0x7C,0x1C,0x2A};// d, S,U,b,U,L,n

/*------------------------------------------------------------------------------------------*/
/*	 								function declare						 				    */
/*------------------------------------------------------------------------------------------*/
void keyboard_test(void);
void keyboard_int(void);
UINT8T key_set(UINT8T ucChar);
/*********************************************************************************************
* name:		keyboard_test
* func:		test 8led
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void keyboard_test(void)
{
	int i, j, k;
	int ii;
	UINT8T ucChar, t;
	char uChar;
	int tag[100],flag=0,key=0;
	//iic_init();
	
	// set EINT2 interrupt handler
        pISR_EINT2 = (int)keyboard_int;
        for(i=0;i<8;i++)
          iic_write(0x70, 0x10+i,0);

	while(1)
	{
                if(uart_getch()=='1')
                  break;
                f_nKeyPress = 0;
		rINTMSK = rINTMSK & (~(BIT_GLOBAL|BIT_EINT2));		// enable EINT2 int
		while(f_nKeyPress == 0);
		iic_read(0x70, 0x1, &ucChar);
		if(uChar ==0) continue;
		if(ucChar!=0)
		{
			if(ucChar==26)
			{
				tag[flag++]=10;
				tag[flag++]=16;
				tag[flag++]=16;
			}
			else if(ucChar==27)
			{

				tag[flag++]=17;
				tag[flag++]=18;
				tag[flag++]=19;
			}
			else if(ucChar==28)
			{

				tag[flag++]=20;
				tag[flag++]=21;
			}
			else if(ucChar==29)
			{

				tag[flag++]=15;
				tag[flag++]=18;
				tag[flag++]=22;
			}
                        /*else if(ucChar==1)
                        {
                                uart_printf("退出实验六和七\n");
                                break;
                        }*/
			else
			{

				ucChar = key_set(ucChar);
			    tag[flag++]=(int*)ucChar;
			}
		   if(flag>8)
		   {
			   for(j=flag%8;j<8+flag%8;j++)
			   {
				     tag[0]=tag[j];
				}
			}
			flag=8;
	     	for(i=flag-1;i>=0;i--)
			{
				if(i<4)
				{
					
					iic_write(0x70, 0x14+i, f_szDigital2[tag[flag-1-i]]);
					
				}
				else if(i < 8)
				{

					iic_write(0x70, 0x10+i-4, f_szDigital2[tag[flag-1-i]]);
					
				}
			}
		   
		 }
		 
	}
}

/*********************************************************************************************
* name:		keyboard_int
* func:		keyboard interrupt handler
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void keyboard_int(void)
{
	UINT8T ucChar;
    delay(1000);
	rINTMSK = rINTMSK | BIT_EINT2;							// disable EINT2 int
    rI_ISPC = BIT_EINT2;
    f_nKeyPress = 1;
}
/*********************************************************************************************
* name:		key_set
* func:		keyboard setting
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
UINT8T key_set(UINT8T ucChar)
{
	switch(ucChar)
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
				ucChar-=1; break;
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
				ucChar-=4; break;
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
				ucChar-=7; break;
		case 25: ucChar = 0xF; break;
		case 26: ucChar = '+'; break;
		case 27: ucChar = '-'; break;
		case 28: ucChar = '*'; break;
		case 29: ucChar = 0xFF; break;
		default: ucChar = 0xFE;
	}
	return ucChar;
}
