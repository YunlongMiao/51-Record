#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
sbit LED1=P1^2;
uint n=0;

void main()		//16位定时器/计数器
{
	TMOD=0x01;		
	TH0=(65536-46080)/256;			
	TL0=(65536-46080)%256;	//50ms
	EA=1;
	ET0=1;
	TR0=1;
	while(1)
	{
		if(n>=20)		//>= 
		{
			LED1=~LED1;
			n=0;
		}
		
	}
}

void T0_timer()interrupt 1
{	
	TH0=(65536-46080)/256;			
	TL0=(65536-46080)%256;	
	n++;
}
