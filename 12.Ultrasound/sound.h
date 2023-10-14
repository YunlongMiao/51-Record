#ifndef __SOUND_H__
#define __SOUND_H__
#include "lcd12864.h"

sbit Trig = P1^0;//发送
sbit Echo = P3^2;//接收


extern void start_sound();

//extern void display_distance();

extern int get_Distance();//获取距离并返回(完整时序)
#endif