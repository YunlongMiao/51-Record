#include "sound.h"



void Timer0Init()		//100微秒@11.0592MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x09;		//工作方式1 Gate置1 INT0—P3^2
	TL0 = 0;		//设置定时初值
	TH0 = 0;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0不会直接计时，INT0置高电位开始启动计时 
}


void start_sound()//定时器初始化
{
	Trig=0;
	Echo=0;
	Timer0Init();
}

int get_Distance()//获取距离并返回(完整时序)
{
	uchar i,timeH,timeL;
	uint t,time,distance;
	
	Trig=1;
	for(i=0; i<10; i++)//延时大于10us，触发
		_nop_();
	
	Trig=0;
	Echo=1;
	
	t=0;
	while(!Echo)//等待低电平结束
	{
		t++;
		_nop_();
		if(t>10000)//防止硬件出错
			return -1;
	}
	
	t=0;
	while(Echo)//等待高电平结束
	{
		t++;
		_nop_();
		if(t>20000)//防止硬件出错
			return -2;
	}
	timeH = TH0;
	timeL = TL0;
	TH0 = 0;//重置
	TL0 = 0;
	
	time = timeH*256+timeL;//获取时间
	if(time>12000)//4m—4/340=0.0117647058823529s
	{
		return -3;
	}
	distance = 1.08                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  *time*1.7/100;//t*340/2=t*17000*10^-6=0.017*t
	
	return distance;
}

//void display_distance()
//{
//	int distance;
//	distance = get_Distance();

//	if(distance<0)
//			LCD_Putstring(3,1,"错误");
//	else
//	{
//		LCD_Setaddress(2,1);
//		LCD_write_dat(0x30+distance/100 );
//		LCD_write_dat(0x30+(distance/10)%10 );
//		LCD_write_dat(0x30+distance%10 );
//	}
//}

















