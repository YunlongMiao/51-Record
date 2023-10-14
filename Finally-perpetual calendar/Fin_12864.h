#ifndef __FIN_12864_H__
#define __FIN_12864_H__
#include"Fin_main.h"
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
//extern void LCD_Setaddress(uchar x, uchar y);

extern void Putstring(uchar addr,uchar *z ); //LCD显示字符数组（文字）

extern void Show_number(uchar addr,uint num);	  //显示数字

//void Show_Chinese(uchar add,uchar mH,uchar mL)	   //显示一个汉字
//{
//	WriteAddText_12864(add);
//	WriteData_12864(mH);
//	WriteData_12864(mL);
//}

//void Show_number(uchar addr,uint num)	  //显示数字
//{
//	char s[6];
//	sprintf(s,"%-05d",num);
//	Show_string(addr , s);
//}

#endif