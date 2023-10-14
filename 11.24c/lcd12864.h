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


//д������
extern void write_com(uchar com);
//д������
extern void write_data(uchar dat);
//LCD��ʼ��
extern void initial_12864();

extern void Delayms(uint n);
//LCD��Ļ��ַ��λ
extern void LCD_Setaddress(uchar x, uchar y);


extern void LCD_Putstring( uchar x, uchar y, uchar *pData );

extern void Putstring(uchar *pData );

extern void Show_number(uchar addr,uint num);	  //��ʾ����
#endif












//void Int_232()	//2323?��??��
//{
//  TMOD=0x20;
//  SCON=0x50;
//  TH1=0xFD;
//  TL1=0xFD;
//  TR1=1;
//  TI=0;
//}

//void TX(uchar TX_char)//��??������?����y?Y
//{
//  SBUF=TX_char;
//  while(!TI);
//  TI=0;
//}

//void TXT()//��??������?��??��y?Y
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