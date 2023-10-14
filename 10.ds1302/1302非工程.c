#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int

sbit PSB = P2 ^ 4;//CS-RS
sbit SCLK = P2 ^ 5;//E
sbit SID = P2 ^ 6;//RW

sbit clk=P0^2;
sbit io=P0^3;
sbit ce=P0^4;

uchar add_write[]={0x8c,0x8a,0x88,0x86,0x84,0x82,0x80};
uchar add_read[]={0x8d,0x8b,0x89,0x87,0x85,0x83,0x81};
uchar tim0[]={20,1,4,13,9,10,26};//年、星期、月、日、时、分秒

uchar tim_bcd[7];//保存读出的时间日期
uchar tim[14];//保存分解开的十位、个位

//将8位二进制数逐位写入
void LCD_sendbyte(uchar dat);
//写入命令
void LCD_write_com(uchar com);
//写入数据
void LCD_write_dat(uchar dat);
//LCD初始化
void LCD_init();
//LCD屏幕地址定位
void LCD_Setaddress(uchar x, uchar y);
////LCD显示字符数组（文字）
//void LCD_Putstring( uchar x, uchar y, uchar *pData );

void display_1302();
void write_1302_8(uchar dat);
void write_1302_byte(uchar add,uchar dat);
uchar read_1302_byte(uchar add);
void ds1302_initial();
void read_1302time();


void Delayms(uint n) 		//延时时间为 1ms * x 晶振是11.0592M
{
	unsigned int i, j;
	for (i = n; i > 0; i--)
		for (j = 110; j > 0; j--);
}


//将8位二进制数逐位写入
void LCD_sendbyte(uchar dat) {
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



//写入命令
void write_com(uchar com) {
	LCD_sendbyte(0xf8);          //送入5个连续的“1“，启动一个周期,11111,RW(0),RS(0),0
	LCD_sendbyte(0xf0 & com);    //取高四位，数据分两次传送，每个字节的内容被送入两个字节,高四位放在第一个字节的高四位
	LCD_sendbyte(0xf0 & (com << 4)); //低四位放在第二个字节的高四位
	Delayms(1);                   //串行不支持读操作，不可检测忙操作，这里用延时替代
}



//写入数据
void write_data(uchar dat) {
	LCD_sendbyte(0xfa);
	LCD_sendbyte(0xf0 & dat);
	LCD_sendbyte(0xf0 & (dat << 4));
	Delayms(1);
}



//LCD初始化
void initial_12864() {
	write_com(0x30);    //选择基本指令集
	write_com(0x0c);    //开显示，无游标，不反白
	write_com(0x01);    //清除显示屏幕，把DDRAM位址计数器调整为00H
	Delayms(1);            //清屏操作时间较长1.6ms 因此加此延时
	write_com(0x02);    //清DDRAM位址归位,此处貌似与清屏重复
	write_com(0x06);    //设定光标右移，整体显示不移动
}


//LCD屏幕地址定位
void LCD_Setaddress(uchar x, uchar y) { //地址从第1行第1列开始不从0开始（y）
	unsigned char addr;
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




////LCD显示字符数组（文字）
//void LCD_Putstring( uchar x, uchar y, uchar *pData ) {
//	LCD_Setaddress(x, y);
//	while (*pData) {
//		write_data( *pData++ );
//	}
//}

/****************写一个字节*************/
/*************写地址，写数据（方式）**************/

void write_1302_8(uchar dat)
{
	uchar i,temp ;
	for(i=0; i<8; i++)
	{
		clk = 0;//时钟信号为低时，数据才能改变，同时为出现上升沿做准备
		temp  = dat&0x01;//取出最低位
		if(temp )
			io =1;//如果这一位为1，io拉高
		else 
			io=0;//如果这一位为0，io拉低
		clk = 1;//出现上升沿，1302读数据
		dat=dat>>1;//为送出下一位做准备
		
	}
}

/*****************写操作******************/
/*************写地址，写数据（目的）**************/

void write_1302_byte(uchar add,uchar dat)
{
	ce =0;//不进行操作时拉低
	clk = 0;//时钟线开始前为低
	ce =1;//拉高启动一次写操作过程
	write_1302_8(add );//送地址字节
	write_1302_8(dat );//送数据字节
	ce =0;//一次写操作的结束
}


/*****************读操作*************/
/*************写地址，读数据（sclk）**************/

uchar read_1302_byte(uchar add)
{
	uchar i,value;
	ce =0;
	clk =0;
	ce =1;//开始一次读操作
	write_1302_8(add );//送地址字节
	for(i=0; i<8; i++)
	{
		clk =0;//读数据时下降沿有效
		value =value >>1;//将上一次保存的数据移到次高位
		if(io)//若检测为1
			value =value|0x80;//将1放到最高位
		clk =1;//接受完一位后将时钟线拉高，为再次出现下降沿做准备
	
	}
	ce =0;//结束一次读操作过程
	return value ;//将读取的1字节数据传递出去
}



/***************初始化**************/
/**********时间、日期的初始值设置**********/

void ds1302_initial()
{
	uchar i,temp;
	for(i=0; i<7; i++)//给寄存器赋初值之前，将日期、时间转换为BCD码
	{
		temp = tim0[i]/10;//取十位
		tim0[i]=temp*16+tim0[i]%10;//bcd 码
	}
	
	write_1302_byte(0x8e,0x00);//给寄存器设置初始值之前，需要去掉写保护
	
	for(i=0; i<7; i++)//每次循环给一个寄存器赋初值
	{
		write_1302_byte(add_write[i],tim0[i]);
	}
	
	write_1302_byte(0x8e,0x80);//给寄存器设置初始值后，需要加上写保护
	

}



/***************读取时间并进行转换*************/

void read_1302time()
{
	uchar i;
	for(i=0; i<7; i++)//每次循环读一个寄存器内容，保存到tim_bcd[]数组中
	{
		tim_bcd[i]=read_1302_byte(add_read[i]);
	}
	
	for(i=0; i<7; i++)//将读出的日期、时间分解为十位和个位
	{
		tim[2*i]=tim_bcd[i]/16;
		tim[2*i+1]=tim_bcd[i]%16;
	}
}





//void Int_232()	//2323?ê??ˉ
//{
//  TMOD=0x20;
//  SCON=0x50;
//  TH1=0xFD;
//  TL1=0xFD;
//  TR1=1;
//  TI=0;
//}

//void TX(uchar TX_char)//′??ú·￠?íêy?Y
//{
//  SBUF=TX_char;
//  while(!TI);
//  TI=0;
//}

//void TXT()//′??ú·￠?í??êy?Y
//{ 
//			uchar i; 
//			for(i=0;i<13;i++)
//			 {
//				 TX(tim[i]);	
//					
//			 }
//}




/***************显示日期和时间**************/

void display_1302()
{
	//Int_232();

	LCD_Setaddress(1,1);
	write_data(0x30+tim[0]);//年
	write_data(0x30+tim[1]);
	write_data('-');
	write_data(0x30+tim[4]);//月
	write_data(0x30+tim[5]);
	write_data('-');
	write_data(0x30+tim[6]);//日
	write_data(0x30+tim[7]);
	write_data(' ');
	write_data(0x30+tim[2]);//周
	write_data(0x30+tim[3]);
	

	LCD_Setaddress(2,1);
	write_data(0x30+tim[8]);
	write_data(0x30+tim[9]);
	write_data(':');
	write_data(0x30+tim[10]);
	write_data(0x30+tim[11]);
	write_data(':');
	write_data(0x30+tim[12]);
	write_data(0x30+tim[13]);
//	TXT();
}



void main()
{
	initial_12864();
	ds1302_initial();
	while(1)
	{	
		read_1302time();
		display_1302();

	}

}