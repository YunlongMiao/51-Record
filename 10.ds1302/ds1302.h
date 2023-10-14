#ifndef _DS1302_H_
#define _DS1302_H_

#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
	
sbit clk=P1^0;
sbit io=P1^1;
sbit ce=P1^2;


extern void ds1302_initial();
extern void display_1302time();
extern void read_1302time();
#endif