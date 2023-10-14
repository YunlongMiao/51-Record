#include "reg52.h"
#include "intrins.h"
#include "string.h"
#include "stdio.h"  
#define uchar unsigned char
#define uint unsigned int
	
sbit PSB = P2 ^ 4;
sbit SCLK = P2 ^ 5;
sbit SID = P2 ^ 6;

sbit scl=P3^6;
sbit sda=P3^7;

sbit clk=P1^0;
sbit io=P1^1;
sbit ce=P1^2;
uchar add_write[]={0x8c,0x8a,0x88,0x86,0x84,0x82,0x80};//寄存器地址
uchar add_read[]={0x8d,0x8b,0x89,0x87,0x85,0x83,0x81};
uchar tim0[]={22,1,2,28,11,24,30,1,1};//年、星期、月、日、时、分、秒、12/24、PM/AM

uchar tim_bcd[7];//保存读出的时间日期
uchar tim[14];//保存分解开的十位、个位


sbit DH = P1 ^ 3; //定义数据线
uchar rec_dat[2];   //用于显示的接收数据数组

sbit KEY_1 = P3^3;	 
sbit KEY_2 = P3^2;	 
sbit KEY_3 = P3^5;	 
sbit KEY_4 = P3^4;

/*天干地支名称*/
 const char *cTianGanDiZhi[] = {"甲子","乙丑","丙寅","丁卯","戊辰","庚午","辛未",
							"壬申","癸酉","甲戌","乙亥","丙子"};

 /*属相名称*/
 const char *cShuXiang[] = {"龙","蛇","马","羊","猴","鸡",
							"狗","猪","鼠","牛","虎","兔"};
 /*农历日期名*/
 const char *cDayName[] = {"*","初一","初二","初三","初四","初五",
						   "初六","初七","初八","初九","初十",
						   "十一","十二","十三","十四","十五",
						   "十六","十七","十八","十九","二十",
						   "廿一","廿二","廿三","廿四","廿五",       
						   "廿六","廿七","廿八","廿九","三十"}; 

 /*农历月份名*/
 const char *cMonName[] = {"*","正","二","三","四","五","六",
						   "七","八","九","十","十一","腊"};

unsigned char shuju[7];		//从1302读取的数据
//月份天数数据表
uchar code day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};
uchar code day_code2[3]={0x111,0x130,0x14e};
//月修正数据表
uchar code table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};
uchar c_moon,year_moon,month_moon,day_moon;


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

//void DHT11_delay_us(uchar n) {
//	while (--n);
//}

//void DHT11_delay_ms(uint z) {
//	uint i, j;
//	for (i = z; i > 0; i--)
//		for (j = 110; j > 0; j--);
//}

//void DHT11_start() {
//	DH = 1;
//	DHT11_delay_us(2);
//	DH = 0;
//	DHT11_delay_ms(30);   //延时18ms以上
//	DH = 1;
//	DHT11_delay_us(30);
//}

//uchar DHT11_rec_byte() {    //接收一个字节
//	uchar i, dat = 0;
//	for (i = 0; i < 8; i++) { //从高到低依次接收8位数据
//		while (!DH);  ////等待50us低电平过去
//		DHT11_delay_us(8);     //延时60us，如果还为高则数据为1，否则为0
//		dat <<= 1;         //移位使正确接收8位数据，数据为0时直接移位
//		if (DH == 1) //数据为1时，使dat加1来接收数据1
//			dat += 1;
//		while (DH); //等待数据线拉低
//	}
//	return dat;
//}

//void DHT11_receive() {    //接收40位的数据
//	uchar R_H, R_L, T_H, T_L, RH, TH, revise;//RL, TL,
//	DHT11_start();
//	if (DH == 0) {
//		while (DH == 0); //等待拉高
//		DHT11_delay_us(40);  //拉高后延时80us
//		R_H = DHT11_rec_byte();  //接收湿度高八位
//		R_L = DHT11_rec_byte();  //接收湿度低八位
//		T_H = DHT11_rec_byte();  //接收温度高八位
//		T_L = DHT11_rec_byte();  //接收温度低八位
//		revise = DHT11_rec_byte(); //接收校正位

//		DHT11_delay_us(25);    //结束

//		if ((R_H + R_L + T_H + T_L) == revise) { //校正
//			RH = R_H;
//			TH = T_H;
//		}
//		/*数据处理，方便显示*/
//				rec_dat[0]=RH;
//        rec_dat[1]=TH;				
//	}
//}

//void display_DHT11()
//{
//		DHT11_receive();
//		Show_number(0x98,rec_dat[1]);
//		write_com(0x99);
//		write_data(0x43);
//		write_data(0x20);
//		Show_number(0x9a,rec_dat[0]);
//		Putstring(0x9b,"RH");
//}

///****************写一个字节*************/
///*************写地址，写数据（方式）**************/

//void write_1302_8(uchar dat)
//{
//	uchar i,temp ;
//	for(i=0; i<8; i++)
//	{
//		clk = 0;//时钟信号为低时，数据才能改变，同时为出现上升沿做准备
//		temp  = dat&0x01;//取出最低位
//		if(temp )
//			io =1;//如果这一位为1，io拉高
//		else 
//			io=0;//如果这一位为0，io拉低
//		clk = 1;//出现上升沿，1302读数据
//		dat=dat>>1;//为送出下一位做准备
//		
//	}
//}

///*****************写操作******************/
///*************写地址，写数据（目的）**************/

//void write_1302_byte(uchar add,uchar dat)
//{
//	ce =0;//不进行操作时拉低
//	clk = 0;//时钟线开始前为低
//	ce =1;//拉高启动一次写操作过程
//	write_1302_8(add );//送地址字节
//	write_1302_8(dat );//送数据字节
//	ce =0;//一次写操作的结束
//}


///*****************读操作*************/
///*************写地址，读数据（sclk）**************/

//uchar read_1302_byte(uchar add)
//{
//	uchar i,value;
//	ce =0;
//	clk =0;
//	ce =1;//开始一次读操作
//	write_1302_8(add );//送地址字节
//	for(i=0; i<8; i++)
//	{
//		clk =0;//读数据时下降沿有效
//		value =value >>1;//将上一次保存的数据移到次高位
//		if(io)//若检测为1
//			value =value|0x80;//将1放到最高位
//		clk =1;//接受完一位后将时钟线拉高，为再次出现下降沿做准备
//	
//	}
//	ce =0;//结束一次读操作过程
//	return value ;//将读取的1字节数据传递出去
//}



///***************初始化**************/
///**********时间、日期的初始值设置**********/

//void ds1302_initial()
//{
//	uchar i,temp;
//	for(i=0; i<7; i++)//给寄存器赋初值之前，将日期、时间转换为BCD码
//	{
//		temp = tim0[i]/10;//取十位
//		tim0[i]=temp*16+tim0[i]%10;//bcd 码
//	}
///****************************************************/	
///************设置显示格式12/24，AM/PM*************************/
///****************************************************/
//		if(tim0[7]==1)
//		{
//			if(tim0[8]==1)
//				tim0[4]|=0xa0;//12小时PM
//			else
//				tim0[4]|=0x80;//12小时AM
//		}
//		else
//		{
//			if(tim0[8]==1)
//				tim0[4]|=0x20;//PM
//			else
//				tim0[4]|=0x00;//AM
//		}
///*****************************************************/
///****************************************************/
///****************************************************/
//	write_1302_byte(0x8e,0x00);//给寄存器设置初始值之前，需要去掉写保护
//	
//	for(i=0; i<7; i++)//每次循环给一个寄存器赋初值
//	{
//		write_1302_byte(add_write[i],tim0[i]);
//	}
//	
//	write_1302_byte(0x8e,0x80);//给寄存器设置初始值后，需要加上写保护
//	

//}



///***************读取时间并进行转换*************/

//void read_1302time()
//{
//	uchar i;
//	for(i=0; i<7; i++)//每次循环读一个寄存器内容，保存到tim_bcd[]数组中
//	{
//		tim_bcd[i]=read_1302_byte(add_read[i]);
///****************************************************/
///****************************************************/
///****************************************************/		
//		if(i==4)
//		{
//			tim_bcd[14] = tim_bcd[4];
//			tim_bcd[4] = tim_bcd[4]& 0x1f;//11111
//		}
///****************************************************/
///****************************************************/
///****************************************************/		
//	}
//	
//	for(i=0; i<7; i++)//将读出的日期、时间分解为十位和个位
//	{
////		distim[i*2]=(tim_bcd[i]/16)*10+tim_bcd[i]%16;
////		distim[i*2+1]=0x20;
//		tim[2*i]=tim_bcd[i]/16;
//		tim[2*i+1]=tim_bcd[i]%16;	
//	}
//}



///***************显示日期和时间**************/

//void display_1302time()
//{
//	
////第一行
//	write_com(0x80);
//	write_data(0x30+tim[0]);//年
//	write_data(0x30+tim[1]);
//	write_data('-');
//	write_data(0x30+tim[4]);//月
//	write_data(0x30+tim[5]);
//	write_data('-');
//	write_data(0x30+tim[6]);//日
//	write_data(0x30+tim[7]);
//	write_data(' ');
////	write_data(0x30+tim[2]);//周
//	write_data(0x30+tim[3]);
//	
///*************************************/
///*************************************/
///*************************************/
//	if(tim_bcd[14]&0x20==0x20)
//		Putstring(0x85,"PM");
//	else
//		Putstring(0x85,"AM");	
///*************************************/
///*************************************/
///*************************************/
//	
////第二行
//	write_com(0x90);
//	write_data(0x30+tim[8]);
//	write_data(0x30+tim[9]);
//	write_data(':');
//	write_data(0x30+tim[10]);
//	write_data(0x30+tim[11]);
//	write_data(':');
//	write_data(0x30+tim[12]);
//	write_data(0x30+tim[13]);

//}


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
	ss=ss-342;//21,363
//	if(ss>=255)
//			mm=read_add(ss-256,1);
//	else
			mm=read_add(ss,0);
//	if(ss==363)
//		mm=0x0a;
//	else if(ss==364)
//		mm=0xd5;
//	else
//		mm=0x41;
	return mm;
}

uchar GetMoonDay(unsigned char month_p,unsigned short table_addr)
/*读取数据表中农历月的大月或小月,如果该月大返回1,小返回0*/
{
	switch (month_p)
	{
		case 1:
			if((year_code(table_addr)&0x08)==0)		return(0);
			else 									return(1); 
		case 2:
			if((year_code(table_addr)&0x04)==0)		return(0);
			else 									return(1);
		case 3:
			if((year_code(table_addr)&0x02)==0)		return(0);
			else 									return(1);
		case 4:
			if((year_code(table_addr)&0x01)==0)		return(0);
			else 								    return(1);
		case 5:
			if((year_code(table_addr)&0x80)==0)	return(0);
			else 									return(1);
		case 6:
			if((year_code(table_addr)&0x40)==0)	return(0);
			else 									return(1);
		case 7:
			if((year_code(table_addr+1)&0x20)==0)	return(0);
			else 									return(1);
		case 8:
			if((year_code(table_addr+1)&0x10)==0)	return(0);
			else 									return(1);
		case 9:
			if((year_code(table_addr+1)&0x08)==0)	return(0);
			else 									return(1);
		case 10:
			if((year_code(table_addr+1)&0x04)==0)	return(0);
			else 									return(1);
		case 11:
			if((year_code(table_addr+1)&0x02)==0)	return(0);
			else 									return(1);
		case 12:
			if((year_code(table_addr+1)&0x01)==0)	return(0);
			else 									return(1);
		case 13:
			if((year_code(table_addr+2)&0x80)==0)	return(0);
			else 									return(1);
	}
	return(0);
}
/*--------------------------------公历转农历函数-----------------------------------------*/

void Conversion(uchar year,uchar month,uchar day)
{                         
	uchar temp1,temp2,temp3,month_p;
	uint temp4,table_addr;
	bit flag2,flag_y,c;	
  	table_addr=(year+0x64-1)*0x3;  //定位数据表地址
	temp1=year_code(table_addr+2)&0x60; //取当年春节所在的公历月份
	temp1=_cror_(temp1,5);
	temp2=year_code(table_addr+2)&0x1f; //取当年春节所在的公历日
	//取当年春节所在的公历日完成 
	//计算当年春年离当年元旦的天数,春节只会在公历1月或2月 
	if(temp1==0x1) 	{ temp3=temp2-1; }  
   	else  		{ temp3=temp2+0x1f-1; }

	// 计算当年春年离当年元旦的天数完成 
	//计算公历日离当年元旦的天数,为了减少运算,用了两个表 
	//day_code1[9],day_code2[3] 
	//如果公历月在九月或前,天数会少于0xff,用表day_code1[9], 
	//在九月后,天数大于0xff,用表day_code2[3] 
	//如输入公历日为8月10日,则公历日离元旦天数为day_code1[8-1]+10-1 
	//如输入公历日为11月10日,则公历日离元旦天数为day_code2[11-10]+10-1 
	if(month<10)	{ temp4=day_code1[month-1]+day-1; }
		else     	{ temp4=day_code2[month-10]+day-1;}

//	if ((month>0x2)&&(year%0x4==0)&&(year!=0)) //此语句造成2000年3月1日—12月31日之间
												//计算农历少算一天，应删除判断语句“&&(year!=0)”
	if ((month>0x2)&&(year%0x4==0))
		temp4+=1; //如果公历月大于2月并且该年的2月为闰月,天数加1
	//计算公历日离当年元旦的天数完成 
	 
	//判断公历日在春节前还是春节后
	if (temp4>=temp3)
	{ 
		temp4-=temp3; month=0x1;
		month_p=0x1;  //month_p为月份指向,公历日在春节前或就是春节当日month_p指向首月
		flag2=GetMoonDay(month_p,table_addr); //检查该农历月为大小还是小月,大月返回1,小月返回0
		flag_y=0;
      if(flag2==0)temp1=0x1d;   //小月29天
      	else    temp1=0x1e;    //大月30天
		temp2=year_code(table_addr)&0xf0;
		temp2=_cror_(temp2,4);  //从数据表中取该年的闰月月份,如为0则该年无闰月
		while(temp4>=temp1)
		{
			temp4-=temp1; month_p+=1;
			if(month==temp2)
	    	{
				flag_y=~flag_y;
         	if(flag_y==0)month+=1;
			}
			else month+=1;
			flag2=GetMoonDay(month_p,table_addr);
         if(flag2==0)temp1=0x1d;
         	else    temp1=0x1e;
		}
		day=temp4+1;
	}
	else
	{  
		temp3-=temp4;
		if (year==0x0){year=0x63;c=1;}
			else	year-=1;
		table_addr-=0x3;
		month=0xc;
		temp2=year_code(table_addr)&0xf0;
		temp2=_cror_(temp2,4);
		if (temp2==0)month_p=0xc; 
			else	month_p=0xd; 
		flag_y=0;
		flag2=GetMoonDay(month_p,table_addr);
		if(flag2==0)temp1=0x1d;
      	else	temp1=0x1e;
		while(temp3>temp1)
		{
			temp3-=temp1;
			month_p-=1;
			if(flag_y==0)month-=1;
         if(month==temp2)flag_y=~flag_y;
         flag2=GetMoonDay(month_p,table_addr);
         if(flag2==0)temp1=0x1d;
            else	temp1=0x1e;
		}
		day=temp1-temp3+1;
	}
	
		c_moon=c;
		year_moon=year;
		month_moon=month;
		day_moon=day;
			Show_number(0x88,year_moon);
			Show_number(0x8a,month_moon);
			Show_number(0x8c,day_moon);
//			Putstring(0x90,cTianGanDiZhi[year_moon%60-8]);
//			Putstring(0x95,cShuXiang[year_moon%12]);
//			Putstring(0x96,cMonName[month_moon]);
//			Putstring(0x97,cDayName[day_moon]);
}
void main()
{
		Delayms(1500);
		initial_12864();		 //12864初始化
		ds1302_initial();
	Conversion(22,3,1);
	while(1)
	{
			display_DHT11();
			read_1302time();
			display_1302time();
	}
}