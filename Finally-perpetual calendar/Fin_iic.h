#ifndef __FIN_IIC_H__
#define __FIN_IIC_H__
#include"Fin_main.h"

sbit scl=P3^6;
sbit sda=P3^7;

extern uchar read_add(uchar address, uchar ss);//读取iic内部数组 year_code[597]（512）
extern void delay1(uint n); //ms
extern uchar year_code(uchar ss);
extern void write_add(uchar address,uchar date, uchar ss);
#endif