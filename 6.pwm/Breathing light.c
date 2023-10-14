#include<reg52.h>
#include<intrins.h>

typedef unsigned char u8;

u8 num=0,sum=0;
bit key=0;

sbit pwm=P1^0;

void main()
{
	TMOD = 0x02; 
	TH0 = 210;    
	TL0 = 210;    
	EA = 1;
	ET0 = 1;
	TR0 = 1;
	while(1);
}

void Tim0() interrupt 1
{
	if(num<=sum)
		pwm=0;
	else
		pwm=1;
	if(num==200)				 //每200次作为一次循环周期
	{
		num=0;
		if(key)					//对当前循环周期所处行为方式进行判定，key为行为储存变量
		 sum--;
		else
		 sum++;
		if(sum==200||sum==0)	//执行方式在一次循环周期结束后变化
		key=!key;
	}
	else
		num++;
}