#ifndef __SOUND_H__
#define __SOUND_H__
#include "lcd12864.h"

sbit Trig = P1^0;//����
sbit Echo = P3^2;//����


extern void start_sound();

//extern void display_distance();

extern int get_Distance();//��ȡ���벢����(����ʱ��)
#endif