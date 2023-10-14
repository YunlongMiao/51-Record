#include"iic.h"
#include"lcd12864.h"

static uchar code year_code[255]={
0x04,0xBA,0x53,0x0A,0x5B,0x48,0x65,0x2B,0xBC, // 2015-2017
0x05,0x2B,0x50,0x0A,0x93,0x45,0x47,0x4A,0xB9, // 2018-2020
0x06,0xAA,0x4C,0x0A,0xd5,0x41,0x24,0xdA,0xB6, // 2021-2023
0x04,0xB6,0x4A,0x69,0x57,0x3d,0x0A,0x4e,0x51, // 2024-2026
0x0d,0x26,0x46,0x5e,0x93,0x3A,0x0d,0x53,0x4d, // 2027-2029
0x05,0xAA,0x43,0x36,0xB5,0x37,0x09,0x6d,0x4B, // 2030-2032
0xB4,0xAe,0xBf,0x04,0xAd,0x53,0x0A,0x4d,0x48, // 2033-2035
0x6d,0x25,0xBC,0x0d,0x25,0x4f,0x0d,0x52,0x44, // 2036-2038
0x5d,0xAA,0x38,0x0B,0x5A,0x4C,0x05,0x6d,0x41, // 2039-2041
0x24,0xAd,0xB6,0x04,0x9B,0x4A,0x7A,0x4B,0xBe, // 2042-2044
0x0A,0x4B,0x51,0x0A,0xA5,0x46,0x5B,0x52,0xBA, // 2045-2047
0x06,0xd2,0x4e,0x0A,0xdA,0x42,0x35,0x5B,0x37, // 2048-2050
0x09,0x37,0x4B,0x84,0x97,0xC1,0x04,0x97,0x53, // 2051-2053
0x06,0x4B,0x48,0x66,0xA5,0x3C,0x0e,0xA5,0x4f, // 2054-2056
0x06,0xB2,0x44,0x4A,0xB6,0x38,0x0A,0xAe,0x4C, // 2057-2059
0x09,0x2e,0x42,0x3C,0x97,0x35,0x0C,0x96,0x49, // 2060-2062
0x7d,0x4A,0xBd,0x0d,0x4A,0x51,0x0d,0xA5,0x45, // 2063-2065
0x55,0xAA,0xBA,0x05,0x6A,0x4e,0x0A,0x6d,0x43, // 2066-2068
0x45,0x2e,0xB7,0x05,0x2d,0x4B,0x8A,0x95,0xBf, // 2069-2071
0x0A,0x95,0x53,0x0B,0x4A,0x47,0x6B,0x55,0x3B, // 2072-2074
0x0A,0xd5,0x4f,0x05,0x5A,0x45,0x4A,0x5d,0x38, // 2075-2077
0x0A,0x5B,0x4C,0x05,0x2B,0x42,0x3A,0x93,0xB6, // 2078-2080
0x06,0x93,0x49,0x77,0x29,0xBd,0x06,0xAA,0x51, // 2081-2083
0x0A,0xd5,0x46,0x54,0xdA,0xBA,0x04,0xB6,0x4e, // 2084-2086
0x0A,0x57,0x43,0x45,0x27,0x38,0x0d,0x26,0x4A, // 2087-2089
0x8e,0x93,0x3e,0x0d,0x52,0x52,0x0d,0xAA,0x47, // 2090-2092
0x66,0xB5,0x3B,0x05,0x6d,0x4f,0x04,0xAe,0x45, // 2093-2095
0x4A,0x4e,0xB9,0x0A,0x4d,0x4C,0x0d,0x15,0x41, // 2096-2098
0x2d,0x92,0xB5, // 2099
};

			
void delay()        //延时应大于4.7us
{ ;;;}


void delay1(uint n) //ms
{ 
	uint i,y; 
	for(i=n;i>0;i--) 
	  for(y=114;y>0;y--); 
}

void start()        //开始发送数据
{  
	 sda=1;
   delay();         //scl在高电平期间，sda由高到低
   scl=1;
   delay();
   sda=0;
   delay();
}

void stop()           //停止发送数据
{  sda=0;             //scl在高电平期间，sda由高到低
   delay();
   scl=1;
   delay();
   sda=1;
   delay();
}


void response()//主机检测从机的应答信号
{ uchar i;
sda = 1;
scl=1;
delay();
while((sda==1)&&i<250) i++; //应答sda为0，非应答为1
scl=0; //释放总线
delay();
}
void ack()
{
	 sda=0;
   delay();
   scl=1;
   delay();
   scl=0;
   delay();
	 sda=1;
   delay();
}
void noack()
{
	 sda=1;
   delay();
   scl=1;
   delay();
   scl=0;
   delay();

}



void write_byte(uchar date)     //写一个字节
{
   uchar i,temp;
   temp=date;
   for(i=0;i<8;i++)
   {  
      scl=0;                    //scl上跳沿写入
      delay();
      sda=temp&0x80;                   
      delay();
      scl=1;
      delay();
		  temp=temp<<1;
			scl=0;
			delay();
   }
		 sda=1;
		 delay();
}

uchar read_byte()
{  
	 uchar i,k;
   scl=0;
   delay();
   sda=1;
   delay();
   for(i=0;i<8;i++)
   {
		  scl=1;
      delay();
      k=k<<1;
		  if(sda)
				k=k|0x01;
      scl=0;
      delay();
   }
   return k;
}

void write_add(uchar address,uchar date)
{  
	 uchar temp;
//		if(ss)
//			temp=0xa2;
//		else
			temp=0xa0;
	 start();
   write_byte(temp);                    //设备地址
   response();
   write_byte(address);
   response();
   write_byte(date);//由于最大寻址为256，所以512字节每次写入2字节
   response();
		delay1(10);
	 stop();
}

uchar read_add(uchar address, uchar ss)
{
	 uchar temp;
 	 uchar date;
	if(ss)
		temp=0xa2;
	else
		temp=0xa0;
   start();
   write_byte(temp);
   response();
   write_byte(address);
   response();
   start();
   write_byte(temp+1);                //1表示接收地址
   response();
   date=read_byte();
		 noack();
		 stop();
		 return date;
	 
}
//void	write_24cxx(unsigned char addr,unsigned char *buff,unsigned char num)//写入一串数据
//{
//	 start();
//   write_byte(0xa0);                    //设备地址
//   response();
//   write_byte(addr);
//   response();
//	while(num--)
//	{
//		write_byte(*buff++);
//		response();
//		delay1(10);//10ms
//	}
//	stop();
//}


//void	read_24cxx(unsigned char addr,unsigned char *buff,unsigned char num)//读入一串数据
//{
//	 unsigned int i;
//	 unsigned char temp;
//	 start();
//   write_byte(0xa0);
//   response();
//   write_byte(addr);
//   response();
//   start();
//   write_byte(0xa1);                //1表示接收地址
//   response();
//	 while(num--)
//	 {
//	  	*buff++=read_byte();
//		  buff[i]=temp;
//		  if(num)
//		  	ack();
//		  else
//				noack();
//		}
//	  stop();
//}
//void	write_24cxx(unsigned char addr,unsigned char *buff,unsigned char num)//写入一串数据
//{
//	unsigned int i;
//	for(i=0;i<num;i++)
//	{
//		write_add(addr++,buff[i],1);
//		delay1(10);//10ms
//	}
//	stop();
//}


//void	read_24cxx(unsigned char addr,unsigned char *buff,unsigned char num)//读入一串数据
//{
//	unsigned int i;
//	unsigned char temp;
//	for(i=0;i<num;i++)
//	{
//		temp=read_add(addr++,1);
//		buff[i]=temp;
//		if(i==num-1)
//			noack();
//		else
//			ack();
//	}
//}
//uchar AT24C02_Write(unsigned char WriteAddr,unsigned char *buffer) 
//{ 
//		unsigned short int BufSize = strlen(buffer); //获取要写入的字符串长度
//		if((Max_Addr-WriteAddr) < BufSize) //空间不足返回0 
//			return 0; 
//		start(); //发I2C起始信号
//		write_byte(0XA0); //写器件地址
//		response(); //等待应答信号
//		write_byte(WriteAddr); //发送地址
//		response(); //等待应答信号
//		while(*buffer++ != 0) 
//		{ 
//				write_byte(*buffer); //发送数据
//				response(); //等待应答信号
//				WriteAddr++; //写入地址加1 
//				if(WriteAddr % Page_Byte == 0) //新的一页
//				{ 
//						stop(); 
//						delay1(6);// AT24C02的写入周期是5ms,延时一定要大于或者等于5ms 
//						start();
//						write_byte(0XA0); 
//						response(); //等待应答信号
//						write_byte(WriteAddr); //发送地址
//						response(); //等待应答信号
//				} 
//		} 
//		stop();
//		delay1(6);
//		return 1; 
//}
//void main()
//{
//	int k,j,a,b,c;
//  initial_12864();
////	 uchar kk;

////  k=0;
//////   
////   write_add(0x00,0x41,0);               //向0单元写入数据0fH，
////	delay1(10);
////  write_add(0x01,0x42,0);
////	delay1(10);
//// write_add(0x02,0x043,0);
////	delay1(10);
//// write_add(0x03,0x044,0);
////	delay1(10);
//// write_add(4,0x045,0);
////   delay1(10);
////   
//  k=read_add(255,0);
////	delay1(10);
//	j=read_add(256,1);
//	a=read_add(257,1);
////	b=read_add(0x03,0);
////	c=read_add(4,0);
////	LCD_Setaddress(1,1);
////	write_data(k);
////	write_data(j);
////	write_data(a);
////	write_data(b);
////	write_data(c);
////	write_data(0x30+kk/10);
////		write_data(0x30+kk%10);
//		write_data(0x30+k/100);
//		write_data(0x30+(k/10)%10);
//		write_data(0x30+k%10);
//		write_data(0x30+j/100);
//		write_data(0x30+(j/10)%10);
//		write_data(0x30+j%10);
//		write_data(0x30+a/100);
//		write_data(0x30+(a/10)%10);
//		write_data(0x30+a%10);
////    P1=read_add(kk,0);                  //低电平灯亮
////  write_24cxx(0x00,sum,7);
////	delay1(2000);
////	AT24C02_Write(0x10,sum);
////	delay1(2000);
////	read_24cxx(0x00,num1,7);
////	read_24cxx(0x10,num2,7);
////   LCD_Setaddress(1,1);
////		for(kk=0; kk<7; kk++)
////		write_data(0x30+num1[kk]);
////	LCD_Setaddress(2,1);
////		for(kk=0; kk<7; kk++)
////		write_data(0x30+num2[kk]);
//    while(1);
// 
//  
//}
//void write_add(uchar address,uchar date, uchar ss)
//{  
//	 start();
//   write_byte(0xa0);                    //设备地址
//   response();
//   write_byte(address);
//   response();
//   write_byte(date);
//   response();
//	 if(ss==0)
//		stop();
//}
////uchar year_code[507]597
void main()
{
	int i,j=0;
	initial_12864();
//for(i=0;i<256; i++)
//	{
//		write_add(i,year_code[i]);
//		delay1(10);
//	}
//	write_add(0,103);//read_add
//	delay1(10);
//	write_add(122,108);
//	delay1(10);
//	write_add(12,210);
//	delay1(10);
	
	Show_number(0x80,read_add(0,0));
	Show_number(0x90,read_add(1,0));
	Show_number(0x88,read_add(2,0));
	for(i=0;i<256; i++)
	{
		Show_number(0x80,read_add(i,0));
		Show_number(0x90,year_code[i]);
		if(read_add(i,0)==year_code[i])
		j++;
		Show_number(0x88,j);
		delay1(1000);
	}
	while(1);
}


//void main()
//{
//	int i;
//	initial_12864();
//		
//	
//}


