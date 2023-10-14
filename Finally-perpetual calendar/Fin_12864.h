#ifndef __FIN_12864_H__
#define __FIN_12864_H__
#include"Fin_main.h"
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
//extern void LCD_Setaddress(uchar x, uchar y);

extern void Putstring(uchar addr,uchar *z ); //LCD��ʾ�ַ����飨���֣�

extern void Show_number(uchar addr,uint num);	  //��ʾ����

//void Show_Chinese(uchar add,uchar mH,uchar mL)	   //��ʾһ������
//{
//	WriteAddText_12864(add);
//	WriteData_12864(mH);
//	WriteData_12864(mL);
//}

//void Show_number(uchar addr,uint num)	  //��ʾ����
//{
//	char s[6];
//	sprintf(s,"%-05d",num);
//	Show_string(addr , s);
//}

#endif