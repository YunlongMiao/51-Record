#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned int
	uint i;
sbit LED_1=P1^0;
sbit LED_2=P1^1;
sbit LED_3=P1^2;
sbit LED_4=P1^3;
sbit LED_5=P1^4;
sbit LED_6=P1^5;
sbit LED_7=P1^6;
sbit LED_8=P1^7;
void delay(uint ms)
{
	while(ms--)
	{
		for(i=0; i<100; i++);
	}
}
void main()
{
	
	while(1)
	{
		LED_1=0;
		delay(100);
		LED_1=1;
		delay(100);
		LED_2=0;
		delay(100);
		LED_2=1;
		delay(100);
		LED_3=0;
		delay(100);
		LED_3=1;
		delay(100);
		LED_4=0;
		delay(100);
		LED_4=1;
		delay(100);
		LED_5=0;
		delay(100);
		LED_5=1;
		delay(100);
		LED_6=0;
		delay(100);
		LED_6=1;
		delay(100);
		LED_7=0;
		delay(100);
		LED_7=1;
		delay(100);
		LED_8=0;
		delay(100);
		LED_8=1;
		delay(100);
	}
}