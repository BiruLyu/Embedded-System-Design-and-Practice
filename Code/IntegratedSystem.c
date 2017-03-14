/*********************************************************************************************
* File：	touchscreen.c
* Author:	embest	
* Desc：	LCD touch screen control function
* History:	
*********************************************************************************************/
#include "44b.h"
#include "44blib.h"
#include "def.h"
#include "touchscreen.h"
#include "lcd.h"
#include "iic.h"

void Integrated_System_Interface(void);
void Integrated_System_test(void);
void keyboard_int(void);


int f_nKeyPress;

/*------------------------------------------------------------------------------------------*/
/*	 								global variables					 				    */
/*------------------------------------------------------------------------------------------*/
unsigned int  f_unMaxX, f_unMaxY;
unsigned int  f_unMinX, f_unMinY;
unsigned int  f_unTouched;	
unsigned int  f_unPosX, f_unPosY;
int globflag=0;


void Integrated_System_Interface(void)
{
	int i;
										// initial LCD controller
	lcd_clr();	// clear screen
	LCD_D_ON;
	
	
	lcd_clr_rect(1,1,319,249,BLUE+40);
	lcd_clr_rect(1,1,319,35,BLUE+10);
	lcd_clr_rect(1,35,319,69,BLUE);
	lcd_clr_rect(1,103,319,137,GREEN);
	lcd_clr_rect(1,69,319,103,GREEN-5);
	lcd_clr_rect(1,137,319,171,RED+22);
	lcd_clr_rect(1,171,319,205,RED+20);
	lcd_clr_rect(1,205,319,239,RED);
	
	lcd_dsp_hz16(35,10,BLACK,"实验一 ARM汇编指令");
        lcd_disp_ascii8x16(99,10,BLACK,"ARM");
        lcd_dsp_hz16(123,10,BLACK,"汇编指令");
        
	lcd_dsp_hz16(35,44,BLACK,"实验二 Thumb汇编指令和ARM工作模式");
        lcd_disp_ascii8x16(99,44,BLACK,"Thumb");
        lcd_dsp_hz16(139,44,BLACK,"汇编指令和");
        lcd_disp_ascii8x16(219,44,BLACK,"ARM");
        lcd_dsp_hz16(243,44,BLACK,"工作模式")；
          
	lcd_dsp_hz16(35,78,BLACK,"实验三 汇编和C语言的相互调用");
        lcd_dsp_hz16(99,78,BLACK,"汇编和");
        lcd_disp_ascii8x16(147,78,BLACk,"C");
        lcd_dsp_hz16(155,78,BLACK,"语言的相互调用");
        
	lcd_dsp_hz16(35,112,BLACK,"实验四 存储器");
        lcd_dsp_hz16(99,112,BLACK,"存储器");
        
	lcd_dsp_hz16(35,146,BLACK,"实验五 中断");
        lcd_dsp_hz16(99,146,BLACK,"中断");
        
	lcd_dsp_hz16(35,180,BLACK,"实验六和七 数码管、键盘控制");
        lcd_dsp_hz16(131,180,BLACK,"数码管和键盘控制");
        
	lcd_dsp_hz16(35,214,BLACK,"实验八和九 液晶显示器和触摸屏");
        lcd_dsp_hz16(131,214,BLACK,"液晶显示器和触摸屏");
	
	
}

/*********************************************************************************************
* name:		touchscreen_int
* func:		TouchScreen interrupt handler function
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void Integrated_System_test(void)
{
	unsigned int unX=0, unY=0;
	unsigned int unTmpX, unTmpY;
	unsigned int sbuf[40];
	unsigned char numbuf1[40],numbuf2[40];
	unsigned int number=0;
	int tag=0;
	int flag=0,cflag=0,nflag=0;
	//int i,j;
	double operand1,operand2,result;
	
	int i, j, k;
	UINT8T  ucChar,t;
	
	
	touchscreen_init();
	lcd_init();
	iic_init();
        
	// set EINT2 interrupt handler
        pISR_EINT2 = (int)keyboard_int;
        for(i=0;i<8;i++)
        iic_write(0x70, 0x10+i,0);

    
    for(;;)
	{
	
	    Integrated_System_Interface();
	    f_nKeyPress = 0;
		rINTMSK = rINTMSK & (~(BIT_GLOBAL|BIT_EINT2));		// enable EINT2 int
		while(f_nKeyPress == 0)
		{
				switch (globflag)
				{
					case 1:
						TEST01();
						break;
					default:
						break;
				}
		}
		iic_read(0x70, 0x1, &ucChar);
		if(ucChar !=0)
		{
			if(ucChar==2)
			{
				uart_printf("实验1\n");
				lcd_clr_rect(1,1,319,35,BLACK);
				delay(1000);
				lcd_clr_rect(1,1,319,35,BLUE+10);
                                lcd_dsp_hz16(35,10,BLACK,"实验一 ARM汇编指令");
                                lcd_disp_ascii8x16(99,10,BLACK,"ARM");
                                lcd_dsp_hz16(123,10,BLACK,"汇编指令");
				globflag=1;	
			}
			else if(ucChar==3)
			{
				uart_printf("实验2\n");
				lcd_clr_rect(1,35,319,69,BLACK);
				delay(1000);
				lcd_clr_rect(1,35,319,69,BLUE);
                                lcd_dsp_hz16(35,44,BLACK,"实验二 Thumb汇编指令和ARM工作模式");
                                lcd_disp_ascii8x16(99,44,BLACK,"Thumb");
                                lcd_dsp_hz16(139,44,BLACK,"汇编指令和");
                                lcd_disp_ascii8x16(219,44,BLACK,"ARM");
                                lcd_dsp_hz16(243,44,BLACK,"工作模式")；				
			}
			else if(ucChar==4)
			{
				uart_printf("实验3\n");
				lcd_clr_rect(1,69,319,103,BLACK);
				delay(1000);
				lcd_clr_rect(1,69,319,103,GREEN-5);
                                lcd_dsp_hz16(35,78,BLACK,"实验三 汇编和C语言的相互调用");
                                lcd_dsp_hz16(99,78,BLACK,"汇编和");
                                lcd_disp_ascii8x16(147,78,BLACk,"C");
                                lcd_dsp_hz16(155,78,BLACK,"语言的相互调用");			
                        }
			else if(ucChar==5)
			{
				uart_printf("实验4\n");
				lcd_clr_rect(1,103,319,137,BLACK);
				delay(1000);
				lcd_clr_rect(1,103,319,137,GREEN);
                                lcd_dsp_hz16(35,112,BLACK,"实验四 存储器");
                                lcd_dsp_hz16(99,112,BLACK,"存储器");			
                        }
			else if(ucChar==9)
			{
				uart_printf("实验5\n");
				lcd_clr_rect(1,137,319,171,BLACK);
				delay(1000);
				lcd_clr_rect(1,137,319,171,RED+22);
                                lcd_dsp_hz16(35,146,BLACK,"实验五 中断");
                                lcd_dsp_hz16(99,146,BLACK,"中断");
			}
			else if(ucChar==10)
			{
				uart_printf("实验6,7\n");
				lcd_clr_rect(1,171,319,205,BLACK);
				delay(1000);
				lcd_clr_rect(1,171,319,205,RED+20);
                                lcd_dsp_hz16(35,180,BLACK,"实验六和七 数码管、键盘控制");
                                lcd_dsp_hz16(131,180,BLACK,"数码管和键盘控制");			
                        }
			else if(ucChar==11)
			{
				uart_printf("实验8,9\n");
				lcd_clr_rect(1,205,319,239,BLACK);
				delay(1000);
				lcd_clr_rect(1,205,319,239,RED);
                                lcd_dsp_hz16(35,214,BLACK,"实验八和九 液晶显示器和触摸屏");
                                lcd_dsp_hz16(131,214,BLACK,"液晶显示器和触摸屏");
			}		
			else if(ucChar==1)
			{
				globflag=0;
			}	
			

			/*ucChar = key_set(ucChar);
			if(ucChar < 10) ucChar += 0x30;
			else if(ucChar < 16) ucChar += 0x37;
			if(ucChar < 255)
				uart_printf("press key %c\n", ucChar);
			if(ucChar == 0xFF)
		  	{
				uart_printf(" press key FUN (exit now)\n\r");
				return;
		   	} */
		   	
			
		 }
		 
	 }
    while(1);
	
	/*while(1)
	{	
	uart_printf("Please touch LCD's left up corner:");
	f_unTouched = 0;
	while(f_unTouched == 0);
	uart_printf("left=%04d, up=%04d\n", f_unPosX, f_unPosY);
	if(f_unPosX<80&&f_unPosY<80)
	{
		f_unMinX=f_unPosX;
		
		}
	
		
	}

	unTmpX = f_unPosX;
	unTmpY = f_unPosY;
		
	uart_printf("Please touch LCD's right bottom corner:");
	f_unTouched = 0;
	while(f_unTouched == 0);
	uart_printf("right=%04d, bottom=%04d\n", f_unPosX, f_unPosY);
	f_unMaxX = f_unPosX>unTmpX? f_unPosX : unTmpX;
	f_unMaxY = f_unPosY>unTmpY? f_unPosY : unTmpY;
	f_unMinX = f_unPosX>unTmpX? unTmpX : f_unPosX;
	f_unMinY = f_unPosY>unTmpY? unTmpY : f_unPosY;
	
	f_unMaxX = 1000;
	f_unMaxY = 1000;
	f_unMinX = 0;
	f_unMinY = 0;

	
    
	for(;;)
	{
		f_unTouched = 0;
		while(f_unTouched == 0);


		//uart_printf("PosX=%04d, PosY=%04d\n", f_unPosX, f_unPosY);
		if(f_unPosX>f_unMaxX || f_unPosX<f_unMinX || f_unPosY>f_unMaxY || f_unPosY<f_unMinY)
			continue;

	//	lcd_clr_rect(unX+2,unY+2,unX+40,unY+20,WHITE);

		unX = 320-((320*(f_unPosX - f_unMinX))/(f_unMaxX - f_unMinX));
		unY = (240*(f_unPosY - f_unMinY))/(f_unMaxY - f_unMinY);
		uart_printf("PosX=%04d, PosY=%04d\n", unX, unY);
		
		if(10<unX&&unX<160&&35<unY&&unY<75)
		{
			tag=0;
			flag=0;
			uart_printf("CE");
		}
		else if(160<unX&&unX<235&&35<unY&&unY<75)
		{
			if(tag!=0)
				tag--;
			uart_printf("退格");
		}
		else if(235<unX&&unX<310&&35<unY&&unY<75)
		{
			sbuf[tag++]='/';
			uart_printf("/");
		}
		else if(10<unX&&unX<85&&75<unY&&unY<115)
		{
			sbuf[tag++]='7';
			uart_printf("7");
				
		}
		else if(85<unX&&unX<160&&75<unY&&unY<115)
		{
			sbuf[tag++]='8';
			uart_printf("8");
		}
		else if(160<unX&&unX<235&&75<unY&&unY<115)
		{
			sbuf[tag++]='9';
			uart_printf("9");
		}
		else if(235<unX&&unX<310&&75<unY&&unY<115)
		{
			sbuf[tag++]='*';
			uart_printf("*");
		}
		else if(10<unX&&unX<85&&115<unY&&unY<155)
		{
			sbuf[tag++]='4';
			uart_printf("4");
		}
		else if(85<unX&&unX<160&&115<unY&&unY<155)
		{
			sbuf[tag++]='5';
			uart_printf("5");
		}
		else if(160<unX&&unX<235&&115<unY&&unY<155)
		{
			sbuf[tag++]='6';
			uart_printf("6");
		}
		else if(235<unX&&unX<310&&115<unY&&unY<155)
		{
			sbuf[tag++]='-';
			uart_printf("-");
		}
		else if(10<unX&&unX<85&&155<unY&&unY<195)
		{
			sbuf[tag++]='1';
			uart_printf("1");
		}
		else if(85<unX&&unX<160&&155<unY&&unY<195)
		{
			sbuf[tag++]='2';
			uart_printf("2");
		}
		else if(160<unX&&unX<235&&155<unY&&unY<195)
		{
			sbuf[tag++]='3';
			uart_printf("3");
		}
		else if(235<unX&&unX<310&&155<unY&&unY<195)
		{
			sbuf[tag++]='+';
			uart_printf("+");
		}
		else if(10<unX&&unX<85&&195<unY&&unY<235)
		{
			sbuf[tag++]='0';
			uart_printf("0");
		}
		else if(85<unX&&unX<160&&195<unY&&unY<235)
		{
			sbuf[tag++]='.';
			uart_printf(".");
		}
		else if(160<unX&&unX<310&&195<unY&&unY<235)//处理“=”号
		{
			uart_printf("=");
			i=0;
			while(sbuf[i]!='+'&&sbuf[i]=='-'&&sbuf[i]=='*'&&sbuf[i]=='/')
			{
				numbuf1[i]=sbuf[i];
				i++;
			}
			if(sbuf[i]=='+')
				flag=1;
			else if(sbuf[i]=='-')
				flag=2;
			else if(sbuf[i]=='*')
				flag=3;
			else if(sbuf[i]=='/')
				flag=4;
			numbuf1[i++]='#';
			operand1=covert(numbuf1);
			for(j=0;i<tag;j++)
			{
				numbuf2[j]=sbuf[i++];
			}
			numbuf2[j]='#';	
			operand2=covert(numbuf2);
			switch(flag)
			{
				case 0:
					result=operand1;
					flag=0;
					break;
				case 1:
					result=operand1+operand2;
					flag=0;
					break;
				case 2:
					result=operand1-operand2;
					flag=0;
					break;
				case 3:
					result=operand1*operand2;
					flag=0;
					break;
				case 4:
					if(operand2==0.0)
						flag=5;
					else 
						result=operand1/operand2;
						flag=0;
						break;
				default:
					flag=5;
					break;
				
			}
			tag=0;
			if((result-(int)result)!=0.0&&flag!=5)
				sprintf(sbuf,"%f", result);	
			else
				sprintf(sbuf,"%d", (int)result);	
						
		}
		if(tag!=0)
		{
			sbuf[tag]='\0';
			lcd_disp_ascii8x16(10,10,BLACK,sbuf);
		}
		else if(flag==5)
		{
			lcd_disp_ascii8x16(10,10,BLACK,"INPUT ERROR");
		}
		else
		{
			lcd_disp_ascii8x16(10,10,BLACK,sbuf);
		}
		//uart_printf("X=%04d, Y=%04d\n", unX, unY);
	////	lcd_clr_rect(81,121,239,59,BLACK);
	////	lcd_clr_rect(82,122,238,58,GREEN);
	//	lcd_clr_rect(82,62,238,118,GREEN);
	//	sprintf(sbuf," X=%04d, Y=%04d\n", unX, unY);
	//	lcd_disp_ascii8x16(90,70,RED,sbuf);


	}

	while(1);		
	*/
}

void keyboard_int(void)
{
	UINT8T ucChar;
    delay(1000);
	rINTMSK = rINTMSK | BIT_EINT2;							// disable EINT2 int
    rI_ISPC = BIT_EINT2;
    f_nKeyPress = 1;
    
}

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
double covert(char s[40])
{
	int i=0,sumi=0;
	double k,sumf=0.0;
	
	for(;s[i]!='#'&&s[i]!='.';i++)
	{
		sumi=sumi*10+s[i]-'0';
	}
	if(s[i]=='.')
	{
		for(i++,k=0.1;s[i]!='#';i++,k=k/10)
			sumf=sumf+(s[i]-'0')*k;
	}
	sumf=sumi+sumf;
	return sumf;
	
}

/*********************************************************************************************
* name:		touchscreen_init
* func:		initialize TouchScreen
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void touchscreen_init(void)
{
#ifndef S3CEV40
	// TSPX(GPC1_Q4(-)) TSPY(GPC3_Q3(-)) TSMY(GPC0_Q2(-)) TSMX(GPC2_Q1(+)) 
	//          1               1                0                 1
	rPCONC = (rPCONC & 0xffffff00) | 0x55;
	rPUPC  = (rPUPE  & 0xfff0); 						// Pull up
	rPDATC = (rPDATC & 0xfff0 ) | 0xe;					// should be enabled
#else	
	// S3CEV40
	// TSPX(GPE4_Q4(+)) TSPY(GPE5_Q3(-)) TSMY(GPE6_Q2(+)) TSMX(GPE7_Q1(-))
    //       0               1                 1                0
	rPCONE = (rPCONE & 0x300ff) | 0x5500;
	rPUPE  = (rPUPE & 0xF);
	rPDATE = 0xb8;
#endif
	delay(100); 
    

    // set interrupt 
#ifndef S3CEV40
	rPUPG = (rPUPG & 0xFE) | 0x1;
    pISR_EINT0=(int)touchscreen_int;       				// set interrupt handler
    rEXTINT = (rEXTINT & 0x7FFFFFF0) | 0x2;           	// falling edge trigger
    rI_ISPC |= BIT_EINT0;            					// clear pending_bit
    rINTMSK =~(BIT_GLOBAL|BIT_EINT0);
#else
    pISR_EINT2=(int)touchscreen_int;       				// set interrupt handler
    rEXTINT = (rEXTINT & 0x7FFFF0FF) | 0x200;             // falling edge trigger
	rI_ISPC |= BIT_EINT2;            					// clear pending_bit
    rINTMSK =~(BIT_GLOBAL|BIT_EINT2);
#endif
    rCLKCON = (rCLKCON & 0x6FFF) | 0x1000;              // enable clock
    rADCPSR = 24;             							// A/D prescaler
}

/*********************************************************************************************
* name:		touchscreen_close
* func:		close TouchScreen
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void touchscreen_close(void)
{
	// Mask interrupt
	rINTMSK |=BIT_GLOBAL|BIT_EINT0;
	pISR_EINT0 = (int)NULL;
}

/*********************************************************************************************
* name:		touchscreen_int
* func:		TouchScreen interrupt handler function
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void touchscreen_int(void)
{
    UINT32T 	unPointX[5], unPointY[6];
    UINT32T		unPosX, unPosY;
    rINTMSK |=BIT_EINT0;
    int   		i;
	delay(500);
#ifndef S3CEV40
	// <X-Position Read>
	// TSPX(GPC1_Q4(+)) TSPY(GPC3_Q3(-)) TSMY(GPC0_Q2(+)) TSMX(GPC2_Q1(-))
    //       0               1                 1                0
	rPDATC = (rPDATC & 0xfff0 ) | 0x9;
	rADCCON= 0x0014;										// AIN5
#else
	// TSPX(GPE4_Q4(+)) TSPY(GPE5_Q3(-)) TSMY(GPE6_Q2(+)) TSMX(GPE7_Q1(-))
    //       0               1                 1                0
	rPDATE =0x68;
	rADCCON=0x1<<2;											// AIN1
#endif	

	delay(100);	                							// delay to set up the next channel
	for(i=0; i<5; i++)
	{
		rADCCON |= 0x1;										// Start X-position A/D conversion
	    while(rADCCON & 0x1 == 1);							// Check if AD conversion starts
    	while((rADCCON & 0x40) == 0);						// Check end of AD conversion
	    unPointX[i] = (0x3ff&rADCDAT);
	}

	// read X-position average value
	unPosX = (unPointX[0]+unPointX[1]+unPointX[2]+unPointX[3]+unPointX[4])/5;
	f_unPosX = unPosX;

#ifndef S3CEV40
    // <Y-Position Read>
	// TSPX(GPC1_Q4(-)) TSPY(GPC3_Q3(+)) TSMY(GPC0_Q2(-)) TSMX(GPC2_Q1(+))
    //       1               0                 0                1
	rPDATC = (rPDATC & 0xfff0 ) | 0x6;
	rADCCON= 0x001C;		        						// AIN70
#else
	// TSPX(GPE4_Q4(-)) TSPY(GPE5_Q3(+)) TSMY(GPE6_Q2(-)) TSMX(GPE7_Q1(+))
    //       1               0                 0                1
	rPDATE =0x98;
	rADCCON=0x0<<2;		        							// AIN0
#endif	
	delay(100);	                							// delay to set up the next channel
	for(i=0; i<5; i++)
	{
    	rADCCON |= 0x1;             						// Start Y-position conversion
	    while(rADCCON & 0x1 == 1);     						// Check if AD conversion starts
    	while((rADCCON & 0x40) == 0); 						// Check end of AD conversion
	    unPointY[i] = (0x3ff&rADCDAT);
	}
	// read Y-position average value
	unPosY = (unPointY[0]+unPointY[1]+unPointY[2]+unPointY[3]+unPointY[4])/5;
	f_unPosY = unPosY;

#ifndef S3CEV40
	rPDATC = (rPDATC & 0xfff0 ) | 0xe;						// should be enabled
#else
	rPDATE = 0xb8;                  						// should be enabled	
#endif	
	delay(1000);

	f_unTouched = 1;
	delay(1000);
	  rI_ISPC |= BIT_EINT0;            					// clear pending_bit
    rINTMSK =~(BIT_GLOBAL|BIT_EINT0);
#ifndef S3CEV40
    rI_ISPC |= BIT_EINT0;            						// clear pending_bit
#else
    rI_ISPC |= BIT_EINT2;            						// clear pending_bit
#endif    
}


