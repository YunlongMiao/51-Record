#ifndef __LCD12864_H__
#define __LCD12864_H__

#include "reg52.h"
#include "intrins.h"
#include "stdio.h"
#define uchar unsigned char
#define uint unsigned int

sbit PSB = P2 ^ 4;
sbit SCLK = P2 ^ 5;
sbit SID = P2 ^ 6;


//写入命令
extern void write_com(uchar com);
//写入数据
extern void write_data(uchar dat);
//LCD初始化
extern void initial_12864();

extern void Delayms(uint n);
//LCD屏幕地址定位
extern void LCD_Setaddress(uchar x, uchar y);


extern void LCD_Putstring( uchar x, uchar y, uchar *pData );

extern void Putstring(uchar *pData );

extern void Show_number(uchar addr,uint num);	  //显示数字
#endif












//void Int_232()	//2323?ê??ˉ
//{
//  TMOD=0x20;
//  SCON=0x50;
//  TH1=0xFD;
//  TL1=0xFD;
//  TR1=1;
//  TI=0;
//}

//void TX(uchar TX_char)//′??ú·￠?íêy?Y
//{
//  SBUF=TX_char;
//  while(!TI);
//  TI=0;
//}

//void TXT()//′??ú·￠?í??êy?Y
//{ 
//			uchar i; 
////			for(i=8;i<14;i++)
////			 {
////				 TX(tim[i]-0x30);	
////					
////			 }
//		TX(tim[14]);
////		for(i=14000; i>0; i--);
//}