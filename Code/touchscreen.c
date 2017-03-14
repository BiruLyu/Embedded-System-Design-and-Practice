/*********************************************************************************************
 * File: touchscreen.c
 * Author:   embest  
 * Desc: LCD touch screen control function
 * History:  
 *********************************************************************************************/
 #include "44b.h"
 #include "44blib.h"
 #include "touchscreen.h"
 #include "lcd.h"
 
 /*------------------------------------------------------------------------------------------*/
 /*                                  global variables                                        */
 /*------------------------------------------------------------------------------------------*/
 unsigned int  f_unMaxX, f_unMaxY;
 unsigned int  f_unMinX, f_unMinY;
 unsigned int  f_unTouched;  
 unsigned int  f_unPosX, f_unPosY;
 
 void lcd_rectangle(void)
 {
     int i;
     //lcd_init();   // initial LCD controller
     lcd_clr();  // clear screen
     LCD_D_ON;
     
     lcd_clr_rect(10,10,310,235,WHITE);
     lcd_clr_rect(10,10,310,35,RED);
     lcd_clr_rect(10,35,310,75,BLUE);
     lcd_clr_rect(10,75,235,195,GREEN);
     lcd_clr_rect(235,35,310,195,RED);
     lcd_clr_rect(10,195,85,235,GREEN);
     lcd_clr_rect(85,195,310,235,BLACK);
     
     
     for(i=0;i<4;i++)
     {
         lcd_clr_rect(15+75*i,40,80+75*i,70,WHITE);  
         
     }
     lcd_clr_rect(15,40,80+75*1,70,WHITE);   
     lcd_disp_ascii8x16(77,47,BLACK,"CE");
     lcd_disp_ascii8x16(185,47,BLACK,"DEL");
     lcd_disp_ascii8x16(43+75*3,47,BLACK,"/");
 
     for(i=0;i<4;i++)
     {
         lcd_clr_rect(15+75*i,80,80+75*i,110,WHITE); 
     }
 
     lcd_disp_ascii8x16(43,87,BLACK,"7");
     lcd_disp_ascii8x16(43+75*1,87,BLACK,"8");
     lcd_disp_ascii8x16(43+75*2,87,BLACK,"9");
     lcd_disp_ascii8x16(43+75*3,87,BLACK,"*");
 
     for(i=0;i<4;i++)
     {
         lcd_clr_rect(15+75*i,120,80+75*i,150,WHITE);    
     }
     lcd_disp_ascii8x16(43,127,BLACK,"4");
     lcd_disp_ascii8x16(43+75*1,127,BLACK,"5");
     lcd_disp_ascii8x16(43+75*2,127,BLACK,"6");
     lcd_disp_ascii8x16(43+75*3,127,BLACK,"-");
 
 
     for(i=0;i<4;i++)
     {
         lcd_clr_rect(15+75*i,160,80+75*i,190,WHITE);    
     }
     lcd_disp_ascii8x16(43,167,BLACK,"1");
     lcd_disp_ascii8x16(43+75*1,167,BLACK,"2");
     lcd_disp_ascii8x16(43+75*2,167,BLACK,"3");
     lcd_disp_ascii8x16(43+75*3,167,BLACK,"+");
     
     lcd_clr_rect(15,200,80,230,WHITE);
     lcd_clr_rect(90,200,155,230,WHITE);
     lcd_clr_rect(165,200,305,230,WHITE);
 
     lcd_disp_ascii8x16(43,207,BLACK,"0");
     lcd_disp_ascii8x16(43+75*1,207,BLACK,".");
     lcd_disp_ascii8x16(231,207,BLACK,"=");
     
 }
 
 /*********************************************************************************************
 * name:     touchscreen_int
 * func:     TouchScreen interrupt handler function
 * para:     none
 * ret:      none
 * modify:
 * comment:      
 ********************************************************************************************/
 void touchscreen_test(void)
 {
     unsigned int unX=0, unY=0;
     unsigned int unTmpX, unTmpY;
     unsigned char sbuf[40];
     unsigned char numbuf1[40],numbuf2[40];
     unsigned int number=0;
     int tag=0;
     int flag=0,cflag=0,nflag=0;
     int i,j;
     double operand1,operand2,result;
     
     //touchscreen_init();
     lcd_rectangle();
     
     for(i=0;i<40;i++)
         sbuf[i]='\0';
         
     where(1)
     {
     uart_printf("Please touch LCD's right up corner:");
     f_unTouched = 0;
     while(f_unTouched == 0);
     uart_printf("left=%04d, up=%04d\n", f_unPosX, f_unPosY);
     //unTmpX = f_unPosX;
     //unTmpY = f_unPosY;
     if(f_unPosX<90& f_unPosY<90)
       {
         f_unMinX=f_unPosX;
         f_unMinY=f_unPosY;
         break;
       }
     }
     
     where(1)
     {   
     uart_printf("Please touch LCD's left bottom corner:");
     f_unTouched = 0;
     while(f_unTouched == 0);
     uart_printf("right=%04d, bottom=%04d\n", f_unPosX, f_unPosY);
     if(f_unPosX>950& f_unPosY>950)
         {
           f_unMaxX=f_unPosX;
           f_unMaxY=f_unPosY;
           break;
         }
     }
     
     //f_unMaxX = f_unPosX>unTmpX? f_unPosX : unTmpX;
     //f_unMaxY = f_unPosY>unTmpY? f_unPosY : unTmpY;
     //f_unMinX = f_unPosX>unTmpX? unTmpX : f_unPosX;
     //f_unMinY = f_unPosY>unTmpY? unTmpY : f_unPosY;
     
 
     
     
     for(;;)
     {
         f_unTouched = 0;
         while(f_unTouched == 0);
 
 
         //uart_printf("PosX=%04d, PosY=%04d\n", f_unPosX, f_unPosY);
         if(f_unPosX>f_unMaxX || f_unPosX<f_unMinX || f_unPosY>f_unMaxY || f_unPosY<f_unMinY)
             continue;
 
     //  lcd_clr_rect(unX+2,unY+2,unX+40,unY+20,WHITE);
 
         unX = (int)(320-((320*((double)f_unPosX - (double)f_unMinX))/((double)f_unMaxX - (double)f_unMinX)));
         unY = (int)(240*((double)f_unPosY - (double)f_unMinY))/((double)f_unMaxY - (double)f_unMinY);
         uart_printf("PosX=%04d, PosY=%04d\n", unX, unY);
         
         if(10<unX&&unX<160&&35<unY&&unY<75)
         {
             tag=0;
             flag=0;
             for(i=0;i<40;i++)
                 sbuf[i]='\0';
             lcd_clr_rect(10,10,310,35,RED);
             lcd_disp_ascii8x16(10,10,BLACK,"");
             uart_printf("CE\n");
         }
         else if(160<unX&&unX<235&&35<unY&&unY<75)
         {
             if(tag!=0)
                 tag--;
             sbuf[tag]='\0';
             lcd_clr_rect(10,10,310,35,RED);
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("DEL\n");
         }
         else if(235<unX&&unX<310&&35<unY&&unY<75)
         {
             sbuf[tag++]='/';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("/\n");
         }
         else if(10<unX&&unX<85&&75<unY&&unY<115)
         {
             sbuf[tag++]='7';
         //  sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("7\n");
                 
         }
         else if(85<unX&&unX<160&&75<unY&&unY<115)
         {
             sbuf[tag++]='8';
         //  sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("8\n");
         }
         else if(160<unX&&unX<235&&75<unY&&unY<115)
         {
             sbuf[tag++]='9';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("9\n");
         }
         else if(235<unX&&unX<310&&75<unY&&unY<115)
         {
             sbuf[tag++]='*';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("*\n");
         }
         else if(10<unX&&unX<85&&115<unY&&unY<155)
         {
             sbuf[tag++]='4';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("4\n");
         }
         else if(85<unX&&unX<160&&115<unY&&unY<155)
         {
             sbuf[tag++]='5';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("5\n");
         }
         else if(160<unX&&unX<235&&115<unY&&unY<155)
         {
             sbuf[tag++]='6';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("6\n");
         }
         else if(235<unX&&unX<310&&115<unY&&unY<155)
         {
             sbuf[tag++]='-';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("-\n");
         }
         else if(10<unX&&unX<85&&155<unY&&unY<195)
         {
             sbuf[tag++]='1';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("1\n");
         }
         else if(85<unX&&unX<160&&155<unY&&unY<195)
         {
             sbuf[tag++]='2';
             //sbuf[tag]='\0';
             
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("2\n");
         }
         else if(160<unX&&unX<235&&155<unY&&unY<195)
         {
             sbuf[tag++]='3';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("3\n");
         }
         else if(235<unX&&unX<310&&155<unY&&unY<195)
         {
             sbuf[tag++]='+';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("+\n");
         }
         else if(10<unX&&unX<85&&195<unY&&unY<235)
         {
             sbuf[tag++]='0';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf("0\n");
         }
         else if(85<unX&&unX<160&&195<unY&&unY<235)
         {
             sbuf[tag++]='.';
             //sbuf[tag]='\0';
             lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             uart_printf(".\n");
         }
         else if(160<unX&&unX<310&&195<unY&&unY<235)//??¡°=¡±?
         {
             uart_printf("=\n");
             i=0;
             while(sbuf[i]!='+'&&sbuf[i]!='-'&&sbuf[i]!='*'&&sbuf[i]!='/')
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
             {
                 sprintf(sbuf,"%f", result); 
                 lcd_clr_rect(10,10,310,35,RED);
         
                 lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             }
             else
             {
                 sprintf(sbuf,"%d", (int)result);
                 lcd_clr_rect(10,10,310,35,RED);
             
                 lcd_disp_ascii8x16(10,10,BLACK,sbuf);
             }   
                         
         }
         //if(tag!=0)
         //{
         //  sbuf[tag]='\0';
         //  lcd_disp_ascii8x16(10,10,BLACK,sbuf);
         //}
         //else if(flag==5)
     //  {
     //      lcd_disp_ascii8x16(10,10,BLACK,"INPUT ERROR");
     //  }
     //  else
     //  {
     //      lcd_disp_ascii8x16(10,10,BLACK,sbuf);
     //  }
         //uart_printf("X=%04d, Y=%04d\n", unX, unY);
     ////    lcd_clr_rect(81,121,239,59,BLACK);
     ////    lcd_clr_rect(82,122,238,58,GREEN);
     //  lcd_clr_rect(82,62,238,118,GREEN);
     //  sprintf(sbuf," X=%04d, Y=%04d\n", unX, unY);
     //  lcd_disp_ascii8x16(90,70,RED,sbuf);
 
 
     }
 
     //while(1);       
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
 * name:     touchscreen_init
 * func:     initialize TouchScreen
 * para:     none
 * ret:      none
 * modify:
 * comment:      
 *********************************************************************************************/
 void touchscreen_init(void)
 {
 #ifndef S3CEV40
     // TSPX(GPC1_Q4(-)) TSPY(GPC3_Q3(-)) TSMY(GPC0_Q2(-)) TSMX(GPC2_Q1(+)) 
     //          1               1                0                 1
     rPCONC = (rPCONC & 0xffffff00) | 0x55;
     rPUPC  = (rPUPE  & 0xfff0);                         // Pull up
     rPDATC = (rPDATC & 0xfff0 ) | 0xe;                  // should be enabled
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
     pISR_EINT0=(int)touchscreen_int;                    // set interrupt handler
     rEXTINT = (rEXTINT & 0x7FFFFFF0) | 0x2;             // falling edge trigger
     rI_ISPC |= BIT_EINT0;                               // clear pending_bit
     rINTMSK =~(BIT_GLOBAL|BIT_EINT0);
 #else
     pISR_EINT2=(int)touchscreen_int;                    // set interrupt handler
     rEXTINT = (rEXTINT & 0x7FFFF0FF) | 0x200;             // falling edge trigger
     rI_ISPC |= BIT_EINT2;                               // clear pending_bit
     rINTMSK =~(BIT_GLOBAL|BIT_EINT2);
 #endif
     rCLKCON = (rCLKCON & 0x6FFF) | 0x1000;              // enable clock
     rADCPSR = 24;                                       // A/D prescaler
 }
 
 /*********************************************************************************************
 * name:     touchscreen_close
 * func:     close TouchScreen
 * para:     none
 * ret:      none
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
 * name:     touchscreen_int
 * func:     TouchScreen interrupt handler function
 * para:     none
 * ret:      none
 * modify:
 * comment:      
 ********************************************************************************************/
 void touchscreen_int(void)
 {
     UINT32T     unPointX[5], unPointY[6];
     UINT32T     unPosX, unPosY;
     rINTMSK |=BIT_EINT0;
     int         i;
     delay(500);
 #ifndef S3CEV40
     // <X-Position Read>
     // TSPX(GPC1_Q4(+)) TSPY(GPC3_Q3(-)) TSMY(GPC0_Q2(+)) TSMX(GPC2_Q1(-))
     //       0               1                 1                0
     rPDATC = (rPDATC & 0xfff0 ) | 0x9;
     rADCCON= 0x0014;                                        // AIN5
 #else
     // TSPX(GPE4_Q4(+)) TSPY(GPE5_Q3(-)) TSMY(GPE6_Q2(+)) TSMX(GPE7_Q1(-))
     //       0               1                 1                0
     rPDATE =0x68;
     rADCCON=0x1<<2;                                         // AIN1
 #endif  
 
     delay(100);                                             // delay to set up the next channel
     for(i=0; i<5; i++)
     {
         rADCCON |= 0x1;                                     // Start X-position A/D conversion
         while(rADCCON & 0x1 == 1);                          // Check if AD conversion starts
         while((rADCCON & 0x40) == 0);                       // Check end of AD conversion
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
     rADCCON= 0x001C;                                        // AIN70
 #else
     // TSPX(GPE4_Q4(-)) TSPY(GPE5_Q3(+)) TSMY(GPE6_Q2(-)) TSMX(GPE7_Q1(+))
     //       1               0                 0                1
     rPDATE =0x98;
     rADCCON=0x0<<2;                                         // AIN0
 #endif  
     delay(100);                                             // delay to set up the next channel
     for(i=0; i<5; i++)
     {
         rADCCON |= 0x1;                                     // Start Y-position conversion
         while(rADCCON & 0x1 == 1);                          // Check if AD conversion starts
         while((rADCCON & 0x40) == 0);                       // Check end of AD conversion
         unPointY[i] = (0x3ff&rADCDAT);
     }
     // read Y-position average value
     unPosY = (unPointY[0]+unPointY[1]+unPointY[2]+unPointY[3]+unPointY[4])/5;
     f_unPosY = unPosY;
 
 #ifndef S3CEV40
     rPDATC = (rPDATC & 0xfff0 ) | 0xe;                      // should be enabled
 #else
     rPDATE = 0xb8;                                          // should be enabled    
 #endif  
     delay(1000);
 
     f_unTouched = 1;
     delay(1000);
       rI_ISPC |= BIT_EINT0;                             // clear pending_bit
     rINTMSK =~(BIT_GLOBAL|BIT_EINT0);
 #ifndef S3CEV40
     rI_ISPC |= BIT_EINT0;                                   // clear pending_bit
 #else
     rI_ISPC |= BIT_EINT2;                                   // clear pending_bit
 #endif    
 }
 
 
 

