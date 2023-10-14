#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
sbit LED1=P1^2;
uint n=0;

void main()		//自动重载8位定时器/计数器）
{
	TMOD=0x02;		
	TL0 = 0xE9;		//25us
	TH0 = 0xE9;
	EA=1;
	ET0=1;
	TR0=1;
	while(1)
	{
		if(n>=40000)
		{
			LED1=~LED1;
			n=0;
		}
		
	}
}

void T0_timer()interrupt 1
{		
	n++;
}