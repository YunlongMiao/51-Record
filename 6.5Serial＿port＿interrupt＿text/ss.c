#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
	

void main()
{
	TMOD=0x20;		//选择定时器1工作方式2-8位自动重装
	TH1=0xe8;
	TL1=0xe8;			//设置波特率=(（2^smod）* F)/(32*12*(256-N)=9600		//smod=0
	REN=1;
	SM0=0;				//选择串口通信模式1
	SM1=1;
	TR1=1;				//定时器1开启
	EA=1;					//中断总开关
	ES=1;					//串行中断允许位
	while(1);
}	

void inter()interrupt 4
{
		uchar s;
		RI=0;		//接收
		s=SBUF;
		RI=0;
		SBUF=s;
		while(!TI);
		TI=0;
}