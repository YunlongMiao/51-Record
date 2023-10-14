#ifndef __FIN_DHT11_H__
#define __FIN_DHT11_H__
#include"Fin_main.h"


sbit DH = P1 ^ 3; //定义数据线

extern void display_DHT11();

#endif