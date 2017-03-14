/*********************************************************************************************
* File£º	lcd.c
* Author:	embest	
* Desc£º	LCD control and display functions
* History:	
*********************************************************************************************/
#include "lcd.h"
#include "44b.h"

/*------------------------------------------------------------------------------------------*/
/*	 					 			constant define							 			    */
/*------------------------------------------------------------------------------------------*/
#define XWIDTH 				6

/*------------------------------------------------------------------------------------------*/
/*	 								global variables						 			    */
/*------------------------------------------------------------------------------------------*/
UINT32T g_unLcdActiveBuffer[LCD_YSIZE][LCD_XSIZE/4];

/*------------------------------------------------------------------------------------------*/
/*	 								extern variables						 			    */
/*------------------------------------------------------------------------------------------*/
extern UINT8T g_ucAscii8x16[];
extern UINT8T g_ucAscii6x8[];
#ifdef CHINESE_VERSION
extern const UINT8T g_ucHZK16[];
#endif

/*********************************************************************************************
* name:		lcd_init()
* func:		Initialize LCD Controller
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_init(void)
{     
	rDITHMODE = 0x12210;
	rDP1_2  = 0xa5a5;      
	rDP4_7  = 0xba5da65;
	rDP3_5  = 0xa5a5f;
	rDP2_3  = 0xd6b;
	rDP5_7  = 0xeb7b5ed;
	rDP3_4  = 0x7dbe;
	rDP4_5  = 0x7ebdf;
	rDP6_7  = 0x7fdfbfe;

	// disable,8B_SNGL_SCAN,WDLY=16clk,WLH=16clk,
	rLCDCON1 = (0x0)|(2<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_COLOR<<12);
	// LINEBLANK=10 (without any calculation) 
	rLCDCON2 = (LINEVAL)|(HOZVAL_COLOR<<10)|(10<<21);  
	rLCDCON3 = 0;

	// 256-color, LCDBANK, LCDBASEU
	rLCDSADDR1 = (0x3<<27) | ( ((unsigned int)g_unLcdActiveBuffer>>22)<<21 ) | M5D((unsigned int)g_unLcdActiveBuffer>>1);
	rLCDSADDR2 = M5D((((unsigned int)g_unLcdActiveBuffer+(SCR_XSIZE*LCD_YSIZE))>>1)) | (MVAL<<21);
	rLCDSADDR3 = (LCD_XSIZE/2) | ( ((SCR_XSIZE-LCD_XSIZE)/2)<<9 );

	//The following value has to be changed for better display.
	rREDLUT  =0xfdb96420; // 1111 1101 1011 1001 0110 0100 0010 0000
	rGREENLUT=0xfdb96420; // 1111 1101 1011 1001 0110 0100 0010 0000
	rBLUELUT =0xfb40;     // 1111 1011 0100 0000

	rLCDCON1=(0x1)|(2<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_COLOR<<12);
	rPDATE=rPDATE&0x0e;
	lcd_clr();
}

/*********************************************************************************************
* name:		lcd_clr()
* func:		clear LCD screen
* para:		none 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_clr(void)
{
	UINT32T i;
	UINT32T *pDisp = (UINT32T*)g_unLcdActiveBuffer;
	
	for (i = 0; i < (SCR_XSIZE * SCR_YSIZE  / 4); i++)
	{
		*pDisp++ = ALLWHITE;
	}
}

/*********************************************************************************************
* name:		lcd_clr_rect()
* func:		fill appointed area with appointed color
* para:		usLeft,usTop,usRight,usBottom -- area's rectangle acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:	also as clear screen function 
*********************************************************************************************/
void lcd_clr_rect(UINT16T usLeft, UINT16T usTop, UINT16T usRight, UINT16T usBottom, UINT8T ucColor)
{
	UINT32T i, j;
	UINT8T  *pDisp = (UINT8T*)g_unLcdActiveBuffer;

//	usTop = 240 - usTop;
	for(i=usTop;i<=usBottom;i++)
	{
		for(j=usLeft;j<=usRight;j++)
		{
			LCD_PutPixel(j, i, ucColor);
		}
	}	
	
}

/*********************************************************************************************
* name:		lcd_draw_box()
* func:		Draw rectangle with appointed color
* para:		usLeft,usTop,usRight,usBottom -- rectangle's acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_draw_box(UINT16T usLeft, UINT16T usTop, UINT16T usRight, UINT16T usBottom, UINT8T ucColor)
{
	lcd_draw_hline(usLeft, usRight,  usTop,    ucColor, 1);
	lcd_draw_hline(usLeft, usRight,  usBottom, ucColor, 1);
	lcd_draw_vline(usTop,  usBottom, usLeft,   ucColor, 1);
	lcd_draw_vline(usTop,  usBottom, usRight,  ucColor, 1);
}

/*********************************************************************************************
* name:		lcd_draw_line()
* func:		Draw line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usX1,usY1 -- line's end point coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_draw_line(UINT16T usX0, UINT16T usY0, UINT16T usX1, UINT16T usY1, UINT8T ucColor, UINT16T usWidth)
{
	UINT16T usDx;
	UINT16T usDy;
	UINT16T y_sign;
	UINT16T x_sign;
	UINT16T decision;
	UINT16T wCurx, wCury, wNextx, wNexty, wpy, wpx;

	if( usY0 == usY1 )
	{
		lcd_draw_hline (usX0, usX1, usY0, ucColor, usWidth);
		return;
	}
	if( usX0 == usX1 )
	{
		lcd_draw_vline (usY0, usY1, usX0, ucColor, usWidth);
		return;
	}
	usDx = abs(usX0 - usX1);
	usDy = abs(usY0 - usY1);
	if( ((usDx >= usDy && (usX0 > usX1)) ||
        ((usDy > usDx) && (usY0 > usY1))) )
    {
        GUISWAP(usX1, usX0);
        GUISWAP(usY1, usY0);
    }
    y_sign = (usY1 - usY0) / usDy;
    x_sign = (usX1 - usX0) / usDx;

    if( usDx >= usDy )
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDx >> 1);
             wCurx <= wNextx; wCurx++, wNextx--, decision += usDy )
        {
            if( decision >= usDx )
            {
                decision -= usDx;
                wCury += y_sign;
                wNexty -= y_sign;
            }
            for( wpy = wCury - usWidth / 2;
                 wpy <= wCury + usWidth / 2; wpy++ )
            {
                LCD_PutPixel(wCurx, wpy, ucColor);
            }

            for( wpy = wNexty - usWidth / 2;
                 wpy <= wNexty + usWidth / 2; wpy++ )
            {
                LCD_PutPixel(wNextx, wpy, ucColor);
            }
        }
    }
    else
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDy >> 1);
             wCury <= wNexty; wCury++, wNexty--, decision += usDx )
        {
            if( decision >= usDy )
            {
                decision -= usDy;
                wCurx += x_sign;
                wNextx -= x_sign;
            }
            for( wpx = wCurx - usWidth / 2;
                 wpx <= wCurx + usWidth / 2; wpx++ )
            {
                LCD_PutPixel(wpx, wCury, ucColor);
            }

            for( wpx = wNextx - usWidth / 2;
                 wpx <= wNextx + usWidth / 2; wpx++ )
            {
                LCD_PutPixel(wpx, wNexty, ucColor);
            }
        }
    }
}

/*********************************************************************************************
* name:		lcd_draw_hline()
* func:		Draw horizontal line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usX1 -- line's end point X-coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_draw_hline(UINT16T usX0, UINT16T usX1, UINT16T usY0, UINT8T ucColor, UINT16T usWidth)
{
	UINT16T usLen;

    if( usX1 < usX0 )
    {
        GUISWAP (usX1, usX0);
    }

    while( (usWidth--) > 0 )
    {
        usLen = usX1 - usX0 + 1;
        while( (usLen--) > 0 )
        {
        	LCD_PutPixel(usX0 + usLen, usY0, ucColor);
        }
        usY0++;
    }
}

/*********************************************************************************************
* name:		lcd_draw_vline()
* func:		Draw vertical line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usY1 -- line's end point Y-coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_draw_vline (UINT16T usY0, UINT16T usY1, UINT16T usX0, UINT8T ucColor, UINT16T usWidth)
{
	UINT16T usLen;

    if( usY1 < usY0 )
    {
        GUISWAP (usY1, usY0);
    }

    while( (usWidth--) > 0 )
    {
        usLen = usY1 - usY0 + 1;
        while( (usLen--) > 0 )
        {
        	LCD_PutPixel(usX0, usY0 + usLen, ucColor);
        }
        usX0++;
    }
}

/*********************************************************************************************
* name:		lcd_disp_ascii8x16()
* func:		display 8x16 ASCII character string 
* para:		usX0,usY0 -- ASCII character string's start point coordinate
*			ForeColor -- appointed color value
*			pucChar   -- ASCII character string
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_disp_ascii8x16(UINT16T x0, UINT16T y0, UINT8T ForeColor, UINT8T * s)
{
	UINT16T i,j,k,x,y,xx;
	UINT8T qm;
	UINT32T ulOffset;
	INT8T ywbuf[16],temp[2];
    
	for( i = 0; i < strlen((const char*)s); i++ )
	{
		if( (UINT8T)*(s+i) >= 161 )
		{
			temp[0] = *(s + i);
			temp[1] = '\0';
			return;
		}
		else
		{
			qm = *(s+i);
			ulOffset = (UINT32T)(qm) * 16;		//Here to be changed tomorrow
			for( j = 0; j < 16; j ++ )
			{
				ywbuf[j] = g_ucAscii8x16[ulOffset + j];
            }

            for( y = 0; y < 16; y++ )
            {
            	for( x = 0; x < 8; x++ ) 
               	{
                	k = x % 8;
			    	if( ywbuf[y]  & (0x80 >> k) )
			       	{
			       		xx = x0 + x + i*8;
			       		LCD_PutPixel(xx, y + y0, (UINT8T)ForeColor);
			       	}
			   	}
            }
		}
	}
}

/*********************************************************************************************
* name:		lcd_disp_ascii6x8()
* func:		display 6x8 ASCII character string 
* para:		usX0,usY0 -- ASCII character string's start point coordinate
*			ForeColor -- appointed color value
*			pucChar   -- ASCII character string
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_disp_ascii6x8(UINT16T usX0, UINT16T usY0,UINT8T ForeColor, UINT8T* pucChar)
{
	UINT32T i,j;
	UINT8T  ucTemp;

	while( *pucChar != 0 )
	{
		for( i=0; i < 8; i++ )
		{
  			ucTemp = g_ucAscii6x8[(*pucChar) * 8 + i];
  			for( j = 0; j < 8; j++ )
  			{
  				if( (ucTemp & (0x80 >> j)) != 0 )
  				{
  					LCD_PutPixel(usX0 + i, usY0 + 8 - j, (UINT8T)ForeColor);
  				}  				
  			}
		}
		usX0 += XWIDTH;
		pucChar++;
	}
}

#ifdef CHINESE_VERSION
/*********************************************************************************************
* name:		lcd_disp_hz16()
* func:		display chinese character string in 16x16 dot array
* para:		usX0,usY0 -- ASCII character string's start point coordinate
*			ForeColor -- appointed color value
*			pucChar   -- ASCII character string
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_disp_hz16(UINT16T x0, UINT16T y0, UINT8T ForeColor, UINT8T *s)
{
	UINT16T i,j,k,x,y,xx;
	UINT8T qm,wm;
	UINT32T ulOffset;
	INT8T hzbuf[32],temp[2];

	for( i = 0; i < strlen((const char*)s); i++ )
	{
		if( ((UINT8T)(*(s+i))) < 161 )
		{
			temp[0] = *(s+i);
			temp[1] = '\0';
			break;
		}
		else
		{
			qm = *(s+i) - 161;
    		wm = *(s + i + 1) - 161;
       		ulOffset = (UINT32T)(qm * 94 + wm) * 32;
			for( j = 0; j < 32; j ++ )
            {
            	hzbuf[j] = g_ucHZK16[ulOffset + j];
            }
            for( y = 0; y < 16; y++ )
            {
	        	for( x = 0; x < 16; x++ ) 
	            {
                	k = x % 8;
				   	if( hzbuf[y * 2 + x / 8]  & (0x80 >> k) )
				    {
				    	xx = x0 + x + i * 8;
				    	LCD_PutPixel( xx, y + y0, (UINT8T)ForeColor);
				    }
			   	}
           	}
		    i++;
		}
	}
}
#endif
