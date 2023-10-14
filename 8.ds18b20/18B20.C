#include <reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

sbit DQ = P1^0;  
sbit PSB=P2^4;
sbit SCLK=P2^5;
sbit SID=P2^6;

uchar Tem[10]={0,0,0,0,0x2E,0,0,0,0,0};
void LCD_Putstring( uchar x, uchar y, uchar *pData );

void Delay(int n)//延时函数 大于10us
{
	while(n--);
}

void Delaynms(uint x) //延时
{
	uint i,j;
 	for(i=0;i<x;i++)
   		for(j=0;j<100;j++);
}


void TX(uchar TX_char)//中断返回
{
  SBUF=TX_char;
  while(!TI);
  TI=0;

}
	
void DS18B20_Init(void)//初始化ds1820
{
	uchar x=0;
	DQ = 1;    
	Delay(8);  //延时
	DQ = 0;    
	Delay(80); //延时 大于 480us
	DQ = 1;    
	Delay(5);
	x=DQ;      //延时后 如果x=0则初始化成功 x=1则初始化失败
	Delay(50);

}

uchar ReadOneChar(void)//读一个字节
{
	uchar i=0;
	uchar dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0;  
		dat>>=1;//00000000
		DQ = 1;  
		if(DQ)
		dat|=0x80;
		Delay(4);
	}
	return(dat);
}


void WriteOneChar(uchar dat)//写一个字节
{
	uchar i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay(2);
		DQ = 1;
		dat>>=1;
	}
}


void ReadTemperature(void)//读取温度
{
	int a=0;
	uchar b=0;
	DS18B20_Init();			//复位
	WriteOneChar(0xCC); // 跳过ROM
	WriteOneChar(0x44); // 温度转换
	DS18B20_Init();
	WriteOneChar(0xCC); //跳过ROM
	WriteOneChar(0xBE); //读取温度寄存器
//	Delay(5000);
	a=ReadOneChar();  //读低8位
	b=ReadOneChar(); //读高8位
	a=(b<<8)|a;

if(a >= 0)
	Tem[0]=0x20;
else
	{
		Tem[0]=0x2d;
		a=a-1;
		a=~a;
  }
		a=a*0.0625*100;//12345
	  Tem[1]=a/10000;
		Tem[2]=a/1000%10;
		Tem[3]=a/100%10;
		Tem[5]=a/10%10;
		Tem[6]=a%10;

}

void Config_Ds18b20(uchar resolution)//ds18b20精度配置
{
	DS18B20_Init();			//复位
	WriteOneChar(0xCC); // 跳过ROM
	WriteOneChar(0x4e); // 写寄存器
	WriteOneChar(0x7d0); // 高温限值125
	WriteOneChar(0x00); // 低温限值0
	WriteOneChar(resolution); //ds18b20精度配置
}






void Delayms(uint n)		//延时时间为 1ms * x 晶振是11.0592M
{
	uint i,j;
	for(i=n;i>0;i--)
		for(j=110;j>0;j--);
	
}

void LCD_sendbyte(uchar dat)
{
   uchar i;
   for(i=0;i<8;i++)
         {
                SCLK=0;
                if(dat&0x80)
                SID=1;
                else SID=0;
                SCLK=1;
                dat=dat<<1;         //左移一位         
         }
}



//写入命令
void LCD_write_com(uchar com)
{        
           LCD_sendbyte(0xf8);          //送入5个连续的“1“，启动一个周期,11111,RW(0),RS(0),0
           LCD_sendbyte(0xf0&com);      //取高四位，数据分两次传送，每个字节的内容被送入两个字节,高四位放在第一个字节的高四位                                 
          LCD_sendbyte(0xf0&(com<<4));  //低四位放在第二个字节的高四位           
          Delayms(1);                   //串行不支持读操作，不可检测忙操作，这里用延时替代
} 



//写入数据
void LCD_write_dat(uchar dat)		
{      
           LCD_sendbyte(0xfa);        
           LCD_sendbyte(0xf0&dat);      
           LCD_sendbyte(0xf0&(dat<<4));            
           Delayms(1);  
}



//LCD初始化
void LCD_init(void)
{        
         LCD_write_com(0x30);    //选择基本指令集            
        LCD_write_com(0x0c);    //开显示，无游标，不反白   
         LCD_write_com(0x01);    //清除显示屏幕，把DDRAM位址计数器调整为00H  
         Delayms(1);            //清屏操作时间较长1.6ms 因此加此延时 
         LCD_write_com(0x02);    //清DDRAM位址归位,此处貌似与清屏重复  
        LCD_write_com(0x06);    //设定光标右移，整体显示不移动  
}


//LCD屏幕地址定位
void LCD_Setaddress(uchar x,uchar y) //地址从第1行第1列开始不从0开始（y）
{            
          uchar addr;
          switch(x)
          {
           case 0: addr=0x80+y;
                   break;
           case 1: addr=0x90+y;
                   break;
           case 2: addr=0x88+y;
                   break;
           case 3: addr=0x98+y;
                   break;
           default : break;
          }
           LCD_write_com(addr);   //字符显示开始地址
}




//LCD显示字符数组（文字）
void LCD_Putstring( uchar x, uchar y, uchar *pData )
{
         LCD_Setaddress(x,y);
         while(*pData)
         {
           LCD_write_dat( *pData++ );
         }
}




void LCD_Putint(uchar x, uchar y )
{
//		uchar a,i;
		LCD_Setaddress(x,y);
		LCD_write_dat(Tem[0]);
		LCD_write_dat(Tem[1]+0x30);
		LCD_write_dat(Tem[2]+0x30);
		LCD_write_dat(Tem[3]+0x30);
		LCD_write_dat(Tem[4]);
		LCD_write_dat(Tem[5]+0x30);
		LCD_write_dat(Tem[6]+0x30);	

}









void Int_return()	
{
  TMOD=0x20;
  SCON=0x50;
  TH1=0xFD;
  TL1=0xFD;
  TR1=1;
  TI=0;
}





void main(void)//1111 1111
{  	
	Config_Ds18b20(0x7f);//ds18b20精度配置	0x1f-9 0x3f-10 0x5f-11 0x7f-12
  Int_return();//串口初始化
	LCD_init();
	LCD_Putstring(1,0,"温度");
	while(1)
	{  
			ReadTemperature();	 //读取温度
//		TX(Tem[0]);
//		TX(Tem[1]+0x30);
//		TX(Tem[2]+0x30);			//点
//		TX(Tem[3]+0x30);
//		TX(Tem[4]);
//		TX(Tem[5]+0x30);
//		TX(Tem[6]+0x30);
//		TX(Tem[7]);
//		TX(Tem[8]);
//		TX(Tem[9]);
		LCD_Putint(1,2 );


	} 	



}
