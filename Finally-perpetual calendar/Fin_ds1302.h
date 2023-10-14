#ifndef __FIN_DS1302_H__
#define __FIN_DS1302_H__

#include"Fin_main.h"
#include "Fin_12864.h"

sbit clk=P1^0;
sbit io=P1^1;
sbit ce=P1^2;

extern void write_1302_byte(uchar add,uchar dat);
extern uchar read_1302_byte(uchar add);
extern void ds1302_initial();
extern void display_1302time();
extern void read_1302time();
#endif