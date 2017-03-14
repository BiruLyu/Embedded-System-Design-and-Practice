/*********************************************************************************************
* File£º	tp.H
* Author:	embest	
* Desc£º	Touch Screen define file
* History:	
*********************************************************************************************/
#include "def.h"
#ifndef __TP_H__
#define __TP_H__

void Integrated_System_test(void);
extern  void Test03();
extern  void Test04(void);
extern  void Test05(void);
extern  void Test0607(void);
extern  void Test0809(void);

void Test01_Interface(void);
void Test02_Interface(void);
void Test03_Interface(void);
void Test04_Interface(void);
void Test05_Interface(void);
void Test0607_Interface(void);
void Test0708_Interface(void);
//void Integrated_System_Interface(void);
void Integrated_System_test(void);
void keyboard_int(void);



void touchscreen_init(void);
void touchscreen_int(void);
void touchscreen_close(void);




#endif /*__TP_H__*/
