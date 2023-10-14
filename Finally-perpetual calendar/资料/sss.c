#include "reg52.h"
#include "intrins.h"
#include "string.h"
#include "stdio.h"  
#define uchar unsigned char
#define uint unsigned int
uchar add_write[]={0x8c,0x8a,0x88,0x86,0x84,0x82,0x80};//寄存器地址
uchar add_read[]={0x8d,0x8b,0x89,0x87,0x85,0x83,0x81};
uchar tim0[]={22,1,2,28,15,24,30,0,1};//年、星期、月、日、时、分、秒、12/24、PM/AM
uchar e=0;
bit w = 0; //调时标志位
uchar rec_dat[2];   //用于显示的接收数据数组
sss();
 void Set_time(unsigned char sel);
uchar tim_bcd[7];//保存读出的时间日期
uchar tim[14];//保存分解开的十位、个位
//uchar distim[14];
void display_1302time();
void write_1302_8(uchar dat);
void write_1302_byte(uchar add,uchar dat);
uchar read_1302_byte(uchar add);
void ds1302_initial();
void read_1302time();
sbit KEY_1 = P3^3;	 
sbit KEY_2 = P3^2;	 
sbit KEY_3 = P3^5;	 
sbit KEY_4 = P3^4;

sbit PSB = P2 ^ 4;
sbit SCLK = P2 ^ 5;
sbit SID = P2 ^ 6;

sbit DH = P1 ^ 3; //定义数据线

sbit scl=P3^6;
sbit sda=P3^7;

sbit clk=P1^0;
sbit io=P1^1;
sbit ce=P1^2;

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



//void LCD_Setaddress(uchar x, uchar y)  //LCD屏幕地址定位,地址从第1行第1列开始不从0开始（y）
//{
//	uchar addr;
//	switch (x) {
//		case 1:
//			addr = 0x80 + y;
//			break;
//		case 2:
//			addr = 0x90 + y;
//			break;
//		case 3:
//			addr = 0x88 + y;
//			break;
//		case 4:
//			addr = 0x98 + y;
//			break;
//		default :
//			break;
//	}
//	write_com(addr);   //字符显示开始地址
//}



void Putstring(uchar addr,uchar *pData ) //LCD显示字符数组（文字）
{
		write_com(addr);
		while (*pData) 
			write_data( *pData++ );

}
void Show_number(uchar addr,uint num)	  //显示数字
{
	char s[6];
	sprintf(s,"%-05d",num);
	Putstring(addr , s);
}

void DHT11_delay_us(uchar n) {
	while (--n);
}

void DHT11_delay_ms(uint z) {
	uint i, j;
	for (i = z; i > 0; i--)
		for (j = 110; j > 0; j--);
}

void DHT11_start() {
	DH = 1;
	DHT11_delay_us(2);
	DH = 0;
	DHT11_delay_ms(30);   //延时18ms以上
	DH = 1;
	DHT11_delay_us(30);
}

uchar DHT11_rec_byte() {    //接收一个字节
	uchar i, dat = 0;
	for (i = 0; i < 8; i++) { //从高到低依次接收8位数据
		while (!DH);  ////等待50us低电平过去
		DHT11_delay_us(8);     //延时60us，如果还为高则数据为1，否则为0
		dat <<= 1;         //移位使正确接收8位数据，数据为0时直接移位
		if (DH == 1) //数据为1时，使dat加1来接收数据1
			dat += 1;
		while (DH); //等待数据线拉低
	}
	return dat;
}

void DHT11_receive() {    //接收40位的数据
	uchar R_H, R_L, T_H, T_L, RH, TH, revise;//RL, TL,
	DHT11_start();
	if (DH == 0) {
		while (DH == 0); //等待拉高
		DHT11_delay_us(40);  //拉高后延时80us
		R_H = DHT11_rec_byte();  //接收湿度高八位
		R_L = DHT11_rec_byte();  //接收湿度低八位
		T_H = DHT11_rec_byte();  //接收温度高八位
		T_L = DHT11_rec_byte();  //接收温度低八位
		revise = DHT11_rec_byte(); //接收校正位

		DHT11_delay_us(25);    //结束

		if ((R_H + R_L + T_H + T_L) == revise) { //校正
			RH = R_H;
			TH = T_H;
		}
		/*数据处理，方便显示*/
				rec_dat[0]=RH;
        rec_dat[1]=TH;				
	}
}

void display_DHT11()
{
		DHT11_receive();
		Show_number(0x98,rec_dat[1]);
		write_com(0x99);
		write_data(0x43);
		write_data(0x20);
		Show_number(0x9a,rec_dat[0]);
		Putstring(0x9b,"RH");
}
			
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

void response()
{  uchar i;
   scl=1;
   delay();
   if((sda==1)&&i<250) i++;        //应答sda为0，非应答为1
   scl=0;                          //释放总线
   delay();
}
//void ack()
//{
//	 sda=0;
//   delay();
//   scl=1;
//   delay();
//   scl=0;
//   delay();
//	 sda=1;
//   delay();
//}
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

uchar year_code(uchar ss)
{
	uchar mm;
	if(ss>=255)
			mm=read_add(ss,1);
	else
			mm=read_add(ss,0);
	return mm;
}




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
/****************************************************/	
/************设置显示格式12/24，AM/PM*************************/
/****************************************************/
		if(tim0[7]==1)
		{
			if(tim0[8]==1)
				tim0[4]|=0xa0;//12小时PM
			else
				tim0[4]|=0x80;//12小时AM
		}
		else
		{
			if(tim0[8]==1)
				tim0[4]|=0x20;//PM
			else
				tim0[4]|=0x00;//AM
		}
/*****************************************************/
/****************************************************/
/****************************************************/
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
/****************************************************/
/****************************************************/
/****************************************************/		
		if(i==4)
		{
			tim_bcd[14] = tim_bcd[4];
			tim_bcd[4] = tim_bcd[4]& 0x1f;//11111
		}
/****************************************************/
/****************************************************/
/****************************************************/		
	}
	
	for(i=0; i<7; i++)//将读出的日期、时间分解为十位和个位
	{
//		distim[i*2]=(tim_bcd[i]/16)*10+tim_bcd[i]%16;
//		distim[i*2+1]=0x20;
		tim[2*i]=tim_bcd[i]/16;
		tim[2*i+1]=tim_bcd[i]%16;	
	}
}



/***************显示日期和时间**************/

void display_1302time()
{
	
//第一行
	write_com(0x80);
	write_data(0x30+tim[0]);//年
	write_data(0x30+tim[1]);
	write_data('-');
	write_data(0x30+tim[4]);//月
	write_data(0x30+tim[5]);
	write_data('-');
	write_data(0x30+tim[6]);//日
	write_data(0x30+tim[7]);
	write_data(' ');
//	write_data(0x30+tim[2]);//周
	write_data(0x30+tim[3]);
	
/*************************************/
/*************************************/
/*************************************/
	if(tim_bcd[14]&0x20==0x20)
		Putstring(0x85,"PM");
	else
		Putstring(0x85,"AM");	
/*************************************/
/*************************************/
/*************************************/
	
//第二行
	write_com(0x90);
	write_data(0x30+tim[8]);
	write_data(0x30+tim[9]);
	write_data(':');
	write_data(0x30+tim[10]);
	write_data(0x30+tim[11]);
	write_data(':');
	write_data(0x30+tim[12]);
	write_data(0x30+tim[13]);

}
///*天干地支名称*/
// const char *cTianGanDiZhi[] = {"甲子","乙丑","丙寅","丁卯","戊辰","庚午","辛未",
//							"壬申","癸酉","甲戌","乙亥","丙子"};

// /*属相名称*/
// const char *cShuXiang[] = {"龙","蛇","马","羊","猴","鸡",
//							"狗","猪","鼠","牛","虎","兔"};
// /*农历日期名*/
// const char *cDayName[] = {"*","初一","初二","初三","初四","初五",
//						   "初六","初七","初八","初九","初十",
//						   "十一","十二","十三","十四","十五",
//						   "十六","十七","十八","十九","二十",
//						   "廿一","廿二","廿三","廿四","廿五",       
//						   "廿六","廿七","廿八","廿九","三十"}; 

// /*农历月份名*/
// const char *cMonName[] = {"*","正","二","三","四","五","六",
//						   "七","八","九","十","十一","腊"}; 
// 
//uchar  moon_shuju[4] = {0,0,0,0};		//用来存储农历数据：月，日
//data uchar set[7] = {0x01,0x20,0x14,0x09,0x11,0x07,0x05};//初值：sec,min,hour,day,month,year,week
//code uchar day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};//月份数据表1-9月份（计算农历春节离元旦的天数）
//code uint day_code2[3]={0x111,0x130,0x14e};							  //10-11-12月份
//bit c_moon;
//data uchar year_moon,month_moon,day_moon;
///*子函数,用于读取数据表中农历月的大月或小月,如果该月为大返回1,为小返回0*/
//bit get_moon_day(uchar month_p,uint table_addr)
//{
//		uchar temp;
//		switch (month_p)
//		 {
//				case 1:
//					{
//						 temp=year_code(table_addr)&0x08;
//						 if (temp==0)
//							 return(0);
//						 else 
//							 return(1);
//					}
//				case 2:{temp=year_code(table_addr)&0x04;
//						 if (temp==0)return(0);else return(1);}
//				case 3:{temp=year_code(table_addr)&0x02;
//						 if (temp==0)return(0);else return(1);}
//				case 4:{temp=year_code(table_addr)&0x01;
//						 if (temp==0)return(0);else return(1);}
//				case 5:{temp=year_code(table_addr+1)&0x80;
//						 if (temp==0) return(0);else return(1);}
//				case 6:{temp=year_code(table_addr+1)&0x40;
//						 if (temp==0)return(0);else return(1);}
//				case 7:{temp=year_code(table_addr+1)&0x20;
//						 if (temp==0)return(0);else return(1);}
//				case 8:{temp=year_code(table_addr+1)&0x10;
//						 if (temp==0)return(0);else return(1);}
//				case 9:{temp=year_code(table_addr+1)&0x08;
//						 if (temp==0)return(0);else return(1);}
//				case 10:{temp=year_code(table_addr+1)&0x04;
//						 if (temp==0)return(0);else return(1);}
//				case 11:{temp=year_code(table_addr+1)&0x02;
//						 if (temp==0)return(0);else return(1);}
//				case 12:{temp=year_code(table_addr+1)&0x01;
//						 if (temp==0)return(0);else return(1);}
//				case 13:{temp=year_code(table_addr+2)&0x80;
//						 if (temp==0)return(0);else return(1);}
//		 }
//}

//void Conversion(bit c,uchar year,uchar month,uchar day)//c=0 为21世纪,c=1 为19世纪 输入输出数据均为BCD数据
//{                         
//		uchar temp1,temp2,temp3,month_p;
//		uint temp4,table_addr;
//		bit flag2,flag_y;
//		uchar Yearl,Moon,Deay;
//		temp1=year/16;   //BCD->hex 先把数据转换为十六进制
//		temp2=year%16;
//		year=temp1*10+temp2;
//		temp1=month/16;
//		temp2=month%16;
//		month=temp1*10+temp2;
//		temp1=day/16;
//		temp2=day%16;
//		day=temp1*10+temp2;
//		//定位数据表地址
//		if(c==0)
//		 {                   
//			 table_addr=(year+0x64-1)*0x3;
//		 }
//		else 
//		 {
//		  	table_addr=(year-1)*0x3;
//		 }
//		//定位数据表地址完成
//		//取当年春节所在的公历月份
//		temp1=year_code(table_addr+2)&0x60; 
//		temp1=_cror_(temp1,5);
//		//取当年春节所在的公历月份完成
//		//取当年春节所在的公历日
//		temp2=year_code(table_addr+2)&0x1f; 
//		//取当年春节所在的公历日完成
//		// 计算当年春年离当年元旦的天数,春节只会在公历1月或2月
//		if(temp1==0x1)
//		 {  
//				temp3=temp2-1;  
//		 }  
//		else
//		 {
//				temp3=temp2+0x1f-1;        
//		 }
//		if (month<10)
//		 { 
//				temp4=day_code1[month-1]+day-1;
//		 }
//		else
//		 {
//				temp4=day_code2[month-10]+day-1;
//		 }
//		if ((month>0x2)&&(year%0x4==0))//如果公历月大于2月并且该年的2月为闰月,天数加1
//		 {  
//				temp4+=1;
//		 }
//		//计算公历日离当年元旦的天数完成
//		//判断公历日在春节前还是春节后
//		if (temp4>=temp3)//公历日在春节后或就是春节当日使用下面代码进行运算
//		 { 
//				temp4-=temp3;
//				month=0x1;
//				month_p=0x1;  //month_p为月份指向,公历日在春节前或就是春节当日month_p指向首月
//				flag2=get_moon_day(month_p,table_addr); //检查该农历月为大小还是小月,大月返回1,小月返回0
//				flag_y=0;
//				if(flag2==0)temp1=0x1d; //小月29天
//				else temp1=0x1e; //大小30天
//				temp2=year_code(table_addr)&0xf0;
//				temp2=_cror_(temp2,4);  //从数据表中取该年的闰月月份,如为0则该年无闰月
//				while(temp4>=temp1)
//				{
//						temp4-=temp1;
//						month_p+=1;
//						if(month==temp2)
//						{
//								flag_y=~flag_y;
//								if(flag_y==0)month+=1;
//						 }
//						else 
//							month+=1;
//						flag2=get_moon_day(month_p,table_addr);
//						if(flag2==0)
//							temp1=0x1d;
//						else 
//							temp1=0x1e;
//				 }
//				day=temp4+1;
//		 }
//		else  //公历日在春节前使用下面代码进行运算
//		 {
//				temp3-=temp4;
//				if (year==0x0)
//						{
//							year=0x63;
//							c=1;
//						}
//				else 
//					year-=1;
//				table_addr-=0x3;
//				month=0xc;
//				temp2=year_code(table_addr)&0xf0;
//				temp2=_cror_(temp2,4);
//				if (temp2==0)
//					month_p=0xc; 
//				else 
//					month_p=0xd; //
//				flag_y=0;
//				flag2=get_moon_day(month_p,table_addr);
//				if(flag2==0)
//					temp1=0x1d;
//				else 
//					temp1=0x1e;
//				while(temp3>temp1)
//			  {
//					temp3-=temp1;
//					month_p-=1;
//					if(flag_y==0)
//						month-=1;
//					if(month==temp2)
//						flag_y=~flag_y;
//					flag2=get_moon_day(month_p,table_addr);
//					if(flag2==0)
//						temp1=0x1d;
//					else 
//						temp1=0x1e;
//				 }
//				day=temp1-temp3+1;
//		 }
//		c_moon=c;                 //HEX->BCD ,运算结束后,把数据转换为BCD数据
//		temp1=year/10;
//		temp1=_crol_(temp1,4);
//		temp2=year%10;
//		Yearl=(year/10)*10+year%10;
//		year_moon=temp1|temp2;
//		 
//		temp1 = month/10;
//		moon_shuju[3] = temp1;
//		temp2 = month%10;
//		moon_shuju[2] = temp2;
//		Moon=moon_shuju[3]*10+moon_shuju[2];
//		 
//		temp1 = day/10;
//		moon_shuju[1] = temp1;
//		temp2 =day%10;
//		moon_shuju[0] = temp2;
//		Deay=moon_shuju[1]*10+moon_shuju[0];
////year_moon	Moon	Deay
//			Putstring(0x90,cTianGanDiZhi[Yearl%60-8]);
//			Putstring(0x91,cShuXiang[Yearl%12]);
//			Putstring(0x92,cMonName[Moon]);
//			Putstring(0x93,cDayName[Deay]);
//}

void main()
{
//	uchar e=0;
//  unsigned char c_sun,year_sun,month_sun,day_sun;    
  KEY_1 = 1;KEY_2 = 1;KEY_3 = 1;KEY_4 = 1;//初始键盘
		Delayms(1500);
		initial_12864();		 //12864初始化
		ds1302_initial();
		while(1)
		{
			display_DHT11();
			read_1302time();
			display_1302time();
			sss();
		}
}





void Set_time(unsigned char sel)//根据选择调整的相应项目加1并写入DS1302
 { 
  signed char address,item;
  signed char max,mini;
	Putstring(0x9d,"调时");
  if(sel==5)  {write_data(0x30+6);address=0x82; max=59;mini=0;}    //分钟6
  if(sel==4)  {write_data(0x30+5);address=0x84; max=23;mini=0;}    //小时5
  if(sel==3)  {write_data(0x30+4);address=0x8a; max=7;mini=1;}    //星期4  
  if(sel==2)  {write_data(0x30+3);address=0x86; max=31;mini=1;}    //日3
  if(sel==1)  {write_data(0x30+2);address=0x88; max=12;mini=1;}    //月2
  if(sel==0)  {write_data(0x30+1);address=0x8c; max=99; mini=0;}    //年1
  //读取1302某地址上的数值转换成10进制赋给item 
  item=((read_1302_byte(address+1))/16)*10 + (read_1302_byte(address+1))%16;
		if(KEY_2 == 0)
		 {
			item++;//数加 1  
		 }
		if(KEY_4 == 0)
		 {
			item--;//数减 1 
		 }
	
  if(item>max) item=mini;//查看数值有效范围   
  if(item<mini) item=max;           
  write_1302_byte(0x8e,0x00);//允许写操作
  write_1302_byte(address,(item/10)*16+item%10);//转换成16进制写入1302
  write_1302_byte(0x8e,0x80);//写保护，禁止写操作 
	read_1302time();

}

sss()
{

	if (w == 0)   //正常走时
	{
//			if(((mm/18)*10+(mm%16))%2==0) 
//			{
//					lcm_w_ss();  
//			}
//			else
//			{
//					lcm_w_nl();
//			}
	}
	else 		//启动调时
	{
	}

	if (KEY_3 == 0) // 设置时间
	{
			Delayms(20);	   //去抖
			if(KEY_3 == 0 && w == 1)  //当是调时状态 本键用于调整下一项
			{
					e++;
					if (e >= 6 )
					{
						e = 0;
					}
					Set_time(e);//调整				
			 }
			if(KEY_3 == 0 && w == 0)  //当是正常状态时就进入调时状态
			 {
//					led_b = 1;//
					write_com(0x01);//清屏
					w=1;	  //进入调时				
					Set_time(e);//调整
			 }
			 while(KEY_3 == 0);//等待键松开 
	}

	if(KEY_1 == 0) // 当在调时状态时就退出调时
	{
			Delayms(20);
			if(KEY_1 == 0 && w == 1)
			{				
					w = 0;	  //退出调时
					e = 0;		//“下一项”计数器清0								
			 }
			if(KEY_1 == 0 && w == 0)
			{				
					write_com(0x01);//清屏
					while(KEY_1 == 0);//等待键松开  
			}
				write_com(0x01);//清屏
				read_1302time();
			while(KEY_1 == 0);//等待键松开  
	}

	if (KEY_2 == 0 && w == 1)// 加减调整
	{
			Delayms(20);
			if(KEY_2 == 0 && w == 1)
			{				
					Set_time(e);//调整
			}
			while(KEY_2 == 0);//等待键松开
	}

	if (KEY_4 == 0 && w == 1) // 加减调整 
	{     	
			Delayms(20);	
			if(KEY_4 == 0 && w == 1)
			{				
					Set_time(e);//调整
			}
			while(KEY_4 == 0);//等待键松开 
	}

}









