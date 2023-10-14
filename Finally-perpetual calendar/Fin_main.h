#ifndef __FIN_MAIN_H__
#define __FIN_MAIN_H__

#include "reg52.h"
#include "intrins.h"
#include "string.h"
#include "stdio.h"  
#define uchar unsigned char
#define uint unsigned int

sbit KEY_1 = P3^3;	 
sbit KEY_2 = P3^2;	 
sbit KEY_3 = P3^5;	 
sbit KEY_4 = P3^4;

#endif

//			Putstring(0x90,cTianGanDiZhi[Yearl%60-8]);
//			Putstring(0x91,cShuXiang[Yearl%12]);
//			Putstring(0x92,cMonName[Moon]);
//			Putstring(0x93,cDayName[Deay]);
//			Putstring(0x96,"Ð¦»°");