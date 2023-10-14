#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
	
//LCD1602 IO设置
#define LCD1602_PORT P0
sbit LCD1602_RS = P2^0;
sbit LCD1602_RW = P2^1;
sbit LCD1602_EN = P2^2;

//****************************************************
//MS延时函数(晶振下测试)
//****************************************************
void LCD1602_delay_ms(uint n)
{
	uint  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<113;j++);
}

//****************************************************
//写指令
//****************************************************
void write_com(uchar com)
{
	LCD1602_RS = 0;
	LCD1602_delay_ms(1);
	LCD1602_EN = 1;
	LCD1602_PORT = com;
	LCD1602_delay_ms(1);
	LCD1602_EN = 0;
}

//****************************************************
//写数据
//****************************************************
void write_data(uchar dat)
{
	LCD1602_RS = 1;
	LCD1602_delay_ms(1);	
	LCD1602_PORT = dat;
	LCD1602_EN = 1;
	LCD1602_delay_ms(1);
	LCD1602_EN = 0;
}

//****************************************************
//连续写字符
//****************************************************
void LCD1602_write_word(uchar *s)
{
	while(*s>0)
	{
		write_data(*s);
		s++;
	}
}

void initial_1602()
{
	LCD1602_EN = 0;
	LCD1602_RW = 0;						//设置为写状态
	write_com(0x38);			//显示模式设定
	write_com(0x0c);			//开关显示、光标有无设置、光标闪烁设置
	write_com(0x06);			//写一个字符后指针加一
	write_com(0x01);			//清屏指令
}

void main()
{
	initial_1602();
	while(1)
	{
		write_com(0x80+0x00);
		LCD1602_write_word("I LOVE YOU!");
	}
}

