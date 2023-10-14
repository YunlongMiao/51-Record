#include <reg52.h>					//52头文件 
 
typedef unsigned char u8;			//别名 
 
u8 Tcount, dutyfactor, direct;		//一个PWM周期内的：周期计数，占空比，方向
 
sbit LED = P1^2;					//位定义，led灯引脚 
 
void InterruptInit();				//中断初始化配置的函数声明
void Time0(); 						//定时器0中断的函数声明 
void breatheLED();					//呼吸灯主程序的函数声明 
 
//主函数 
void main() {
	InterruptInit();				//初始化中断配置 
	
	//循环实现呼吸灯功能
	//PWM周期：20ms；占空比：0~100；占空比每一份：200us；一次呼吸灯效果(暗->亮->暗)：4s
	while(1) {
		breatheLED();				//呼吸灯效果实现				
	}
}
 
//中断初始化配置
void InterruptInit() {
	TMOD &= 0xf0;					//定时器0设置清除
	TMOD |= 0x02;					//定时器0，8位自动重装载
	
	//初值
	TH0 = 0x48;						//200us定时，2^8-11.0592M / 12 * 200us
	TL0 = 0x48;						//自动重装载，低八位等于高八位
	
	ET0 = 1;						//打开定时器0中断允许
	EA  = 1;						//打开总中断
	
	TR0 = 1;						//启动定时器0
}
 
//定时器0中断
void Time0() interrupt 1 {
	//200us初值重装
	TH0 = 0x48;						
	TL0 = 0x48;
	
	Tcount++;						//一个PWM周期内，计数
}
 
//呼吸灯主程序
void breatheLED() {
	if(Tcount >= 100) {				//一个PWM周期(100次*200us)
		Tcount = 0;					//下一个PWM周期内，重新计数
		
		//根据方向判断占空比增加或减少
		if(direct == 0) {			//正方向，占空比增加
			dutyfactor++;			//下一个PWM周期，占空比增加1
		} else if(direct == 1) {	//负方向，占空比减少
			dutyfactor--;			//下一个PWM周期，占空比减少1
		}
	}
	
	//根据占空比值判断方向是否需要改变
	if(dutyfactor <= 0) {			//占空比减少到0
		direct = 0;					//正方向，占空比开始增加
	} else if(dutyfactor >= 100) {	//占空比增加到100
		direct = 1;					//负方向，占空比开始减少
	}
	
	//占空比：通电时间相对于总时间所占的比例（此程序中：低电平时间相对于PWM周期所占的比例） 
	//一个PWM周期计数100次，占空比相当于低电平的计数周期次数。即：0~占空比：低电平，灯亮；占空比~100：高电平，灯灭 
	//在一个PWM周期内，根据周期计数是否小于占空比，判断LED灯的亮灭
	if(Tcount < dutyfactor) {		//周期计数值<占空比
		LED = 0;					//当前PWM周期的当前计数周期内灯亮
	} else {
		LED = 1;					//当前PWM周期的当前计数周期内灯灭
	}
}