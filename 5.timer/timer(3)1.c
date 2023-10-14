#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
sbit LED1=P1^0;
sbit LED2=P1^2;
uint n=0,m=0;
void main()		
{
	TMOD=0x03;		
	TL0 = 0xE9;		//25us
	TH0 = 0xE9;
	EA=1;
	ET0=1;
	TR0=1;
	ET1=1;	
	TR1=1;
	while(1)
	{
		if(n>=20000)//0.5s
		{
			n=0;
			LED1=~LED1;
		}
		if(m>=40000)//1s
		{
			m=0;
			LED2=~LED2;
			
		}
		
	}
}

void TL0_timer()interrupt 1
{	
	TL0 = 0xE9;		//25us
	n++;
}
void TH0_timer()interrupt 3
{
	TH0 = 0xE9;	
	m++;
}
