#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
sbit LED1=P1^2;
uint n=0;

void main()		//13位定时器/计数器
{
	TMOD=0x00;	//定时器工作方式寄存器		
	TH0=144;	//(8192-3584)/32;			
	TL0=0;		//(8192-3584)%32;	//5ms
	EA=1;			////开总中断
	ET0=1;		 //开定时器 0 中断
	TR0=1;		 //启动定时器 0
	while(1)
	{
		if(n>=200)
		{
			LED1=~LED1;
			n=0;
		}
		
	}
}

void T0_timer()interrupt 1
{	
	TH0=144;		//(8192-3584)/32;			
	TL0=0;			//(8192-3584)%32;	
	n++;
}