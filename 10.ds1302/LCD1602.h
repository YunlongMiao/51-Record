#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
	
//LCD1602 IO设置
#define LCD1602_PORT P0
sbit LCD1602_RS = P2^0;
sbit LCD1602_RW = P2^1;
sbit LCD1602_EN = P2^2;

//函数或者变量声明
extern void LCD1602_delay_ms(uint n);
extern void write_com(uchar com);
extern void write_data(uchar dat);
extern void LCD1602_write_word(uchar *s);
extern void initial_1602();

#endif