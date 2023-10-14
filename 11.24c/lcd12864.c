#include"lcd12864.h"
//uchar sum[]={1,1,1,1,1,1,1,1};
//uchar num[8];
//将8位二进制数逐位写入
void LCD_sendbyte(uchar dat);
//写入命令
void write_com(uchar com);
//写入数据
void write_data(uchar dat) ;
//LCD初始化
void initial_12864();
//LCD屏幕地址定位
void LCD_Setaddress(uchar x, uchar y);
//LCD显示字符数组（文字）
void LCD_Putstring( uchar x, uchar y, uchar *pData );


void Delayms(uint n) 		//延时时间为 1ms * x 晶振是11.0592M
{
	unsigned int i, j;
	for (i = n; i > 0; i--)
		for (j = 110; j > 0; j--);
}



void LCD_sendbyte(uchar dat) //将8位二进制数逐位写入
{
	unsigned char i;
	for (i = 0; i < 8; i++) {
		SCLK = 0;
		if (dat & 0x80)
			SID = 1;
		else
			SID = 0;
		SCLK = 1;
		dat = dat << 1;     //左移一位
	}
}




void write_com(uchar com) //写入命令
{
	LCD_sendbyte(0xf8);          //送入5个连续的“1“，启动一个周期,11111,RW(0),RS(0),0
	LCD_sendbyte(0xf0 & com);    //取高四位，数据分两次传送，每个字节的内容被送入两个字节,高四位放在第一个字节的高四位
	LCD_sendbyte(0xf0 & (com << 4)); //低四位放在第二个字节的高四位
	Delayms(1);                   //串行不支持读操作，不可检测忙操作，这里用延时替代
}




void write_data(uchar dat) //写入数据
{
	LCD_sendbyte(0xfa);
	LCD_sendbyte(0xf0 & dat);
	LCD_sendbyte(0xf0 & (dat << 4));
	Delayms(1);
}




void initial_12864()//LCD初始化
{
	write_com(0x30);    //选择基本指令集
	write_com(0x0c);    //开显示，无游标，不反白
	write_com(0x01);    //清除显示屏幕，把DDRAM位址计数器调整为00H
	Delayms(1);            //清屏操作时间较长1.6ms 因此加此延时
	write_com(0x02);    //清DDRAM位址归位,此处貌似与清屏重复
	write_com(0x06);    //设定光标右移，整体显示不移动
}



void LCD_Setaddress(uchar x, uchar y)  //LCD屏幕地址定位,地址从第1行第1列开始不从0开始（y）
{
	uchar addr;
	switch (x) {
		case 1:
			addr = 0x80 + y - 1;
			break;
		case 2:
			addr = 0x90 + y - 1;
			break;
		case 3:
			addr = 0x88 + y - 1;
			break;
		case 4:
			addr = 0x98 + y - 1;
			break;
		default :
			break;
	}
	write_com(addr);   //字符显示开始地址
}





void LCD_Putstring( uchar x, uchar y, uchar *pData ) //LCD显示字符数组（地址文字）
{
	LCD_Setaddress(x, y);
	while (*pData) {
		write_data( *pData++ );
	}
}


void Putstring(uchar *pData ) //LCD显示字符数组（文字）
{	
	while (*pData) 
		write_data( *pData++ );

}
void Show_number(uchar addr,uint num)	  //显示数字
{
	char s[6];
	sprintf(s,"%-05d",num);
	write_com(addr);
	Putstring(s);
}






