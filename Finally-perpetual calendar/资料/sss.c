#include "reg52.h"
#include "intrins.h"
#include "string.h"
#include "stdio.h"  
#define uchar unsigned char
#define uint unsigned int
uchar add_write[]={0x8c,0x8a,0x88,0x86,0x84,0x82,0x80};//�Ĵ�����ַ
uchar add_read[]={0x8d,0x8b,0x89,0x87,0x85,0x83,0x81};
uchar tim0[]={22,1,2,28,15,24,30,0,1};//�ꡢ���ڡ��¡��ա�ʱ���֡��롢12/24��PM/AM
uchar e=0;
bit w = 0; //��ʱ��־λ
uchar rec_dat[2];   //������ʾ�Ľ�����������
sss();
 void Set_time(unsigned char sel);
uchar tim_bcd[7];//���������ʱ������
uchar tim[14];//����ֽ⿪��ʮλ����λ
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

sbit DH = P1 ^ 3; //����������

sbit scl=P3^6;
sbit sda=P3^7;

sbit clk=P1^0;
sbit io=P1^1;
sbit ce=P1^2;

void Delayms(uint n) 		//��ʱʱ��Ϊ 1ms * x ������11.0592M
{
	unsigned int i, j;
	for (i = n; i > 0; i--)
		for (j = 110; j > 0; j--);
}



void LCD_sendbyte(uchar dat) //��8λ����������λд��
{
	unsigned char i;
	for (i = 0; i < 8; i++) {
		SCLK = 0;
		if (dat & 0x80)
			SID = 1;
		else
			SID = 0;
		SCLK = 1;
		dat = dat << 1;     //����һλ
	}
}




void write_com(uchar com) //д������
{
	LCD_sendbyte(0xf8);          //����5�������ġ�1��������һ������,11111,RW(0),RS(0),0
	LCD_sendbyte(0xf0 & com);    //ȡ����λ�����ݷ����δ��ͣ�ÿ���ֽڵ����ݱ����������ֽ�,����λ���ڵ�һ���ֽڵĸ���λ
	LCD_sendbyte(0xf0 & (com << 4)); //����λ���ڵڶ����ֽڵĸ���λ
	Delayms(1);                   //���в�֧�ֶ����������ɼ��æ��������������ʱ���
}




void write_data(uchar dat) //д������
{
	LCD_sendbyte(0xfa);
	LCD_sendbyte(0xf0 & dat);
	LCD_sendbyte(0xf0 & (dat << 4));
	Delayms(1);
}




void initial_12864()//LCD��ʼ��
{
	write_com(0x30);    //ѡ�����ָ�
	write_com(0x0c);    //����ʾ�����α꣬������
	write_com(0x01);    //�����ʾ��Ļ����DDRAMλַ����������Ϊ00H
	Delayms(1);            //��������ʱ��ϳ�1.6ms ��˼Ӵ���ʱ
	write_com(0x02);    //��DDRAMλַ��λ,�˴�ò���������ظ�
	write_com(0x06);    //�趨������ƣ�������ʾ���ƶ�
}



//void LCD_Setaddress(uchar x, uchar y)  //LCD��Ļ��ַ��λ,��ַ�ӵ�1�е�1�п�ʼ����0��ʼ��y��
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
//	write_com(addr);   //�ַ���ʾ��ʼ��ַ
//}



void Putstring(uchar addr,uchar *pData ) //LCD��ʾ�ַ����飨���֣�
{
		write_com(addr);
		while (*pData) 
			write_data( *pData++ );

}
void Show_number(uchar addr,uint num)	  //��ʾ����
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
	DHT11_delay_ms(30);   //��ʱ18ms����
	DH = 1;
	DHT11_delay_us(30);
}

uchar DHT11_rec_byte() {    //����һ���ֽ�
	uchar i, dat = 0;
	for (i = 0; i < 8; i++) { //�Ӹߵ������ν���8λ����
		while (!DH);  ////�ȴ�50us�͵�ƽ��ȥ
		DHT11_delay_us(8);     //��ʱ60us�������Ϊ��������Ϊ1������Ϊ0
		dat <<= 1;         //��λʹ��ȷ����8λ���ݣ�����Ϊ0ʱֱ����λ
		if (DH == 1) //����Ϊ1ʱ��ʹdat��1����������1
			dat += 1;
		while (DH); //�ȴ�����������
	}
	return dat;
}

void DHT11_receive() {    //����40λ������
	uchar R_H, R_L, T_H, T_L, RH, TH, revise;//RL, TL,
	DHT11_start();
	if (DH == 0) {
		while (DH == 0); //�ȴ�����
		DHT11_delay_us(40);  //���ߺ���ʱ80us
		R_H = DHT11_rec_byte();  //����ʪ�ȸ߰�λ
		R_L = DHT11_rec_byte();  //����ʪ�ȵͰ�λ
		T_H = DHT11_rec_byte();  //�����¶ȸ߰�λ
		T_L = DHT11_rec_byte();  //�����¶ȵͰ�λ
		revise = DHT11_rec_byte(); //����У��λ

		DHT11_delay_us(25);    //����

		if ((R_H + R_L + T_H + T_L) == revise) { //У��
			RH = R_H;
			TH = T_H;
		}
		/*���ݴ���������ʾ*/
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
			
void delay()        //��ʱӦ����4.7us
{ ;;;}


void delay1(uint n) //ms
{ 
	uint i,y; 
	for(i=n;i>0;i--) 
	  for(y=114;y>0;y--); 
}

void start()        //��ʼ��������
{  
	 sda=1;
   delay();         //scl�ڸߵ�ƽ�ڼ䣬sda�ɸߵ���
   scl=1;
   delay();
   sda=0;
   delay();
}

void stop()           //ֹͣ��������
{  sda=0;             //scl�ڸߵ�ƽ�ڼ䣬sda�ɸߵ���
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
   if((sda==1)&&i<250) i++;        //Ӧ��sdaΪ0����Ӧ��Ϊ1
   scl=0;                          //�ͷ�����
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



void write_byte(uchar date)     //дһ���ֽ�
{
   uchar i,temp;
   temp=date;
   for(i=0;i<8;i++)
   {  
      scl=0;                    //scl������д��
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
   write_byte(temp+1);                //1��ʾ���յ�ַ
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




/****************дһ���ֽ�*************/
/*************д��ַ��д���ݣ���ʽ��**************/

void write_1302_8(uchar dat)
{
	uchar i,temp ;
	for(i=0; i<8; i++)
	{
		clk = 0;//ʱ���ź�Ϊ��ʱ�����ݲ��ܸı䣬ͬʱΪ������������׼��
		temp  = dat&0x01;//ȡ�����λ
		if(temp )
			io =1;//�����һλΪ1��io����
		else 
			io=0;//�����һλΪ0��io����
		clk = 1;//���������أ�1302������
		dat=dat>>1;//Ϊ�ͳ���һλ��׼��
		
	}
}

/*****************д����******************/
/*************д��ַ��д���ݣ�Ŀ�ģ�**************/

void write_1302_byte(uchar add,uchar dat)
{
	ce =0;//�����в���ʱ����
	clk = 0;//ʱ���߿�ʼǰΪ��
	ce =1;//��������һ��д��������
	write_1302_8(add );//�͵�ַ�ֽ�
	write_1302_8(dat );//�������ֽ�
	ce =0;//һ��д�����Ľ���
}


/*****************������*************/
/*************д��ַ�������ݣ�sclk��**************/

uchar read_1302_byte(uchar add)
{
	uchar i,value;
	ce =0;
	clk =0;
	ce =1;//��ʼһ�ζ�����
	write_1302_8(add );//�͵�ַ�ֽ�
	for(i=0; i<8; i++)
	{
		clk =0;//������ʱ�½�����Ч
		value =value >>1;//����һ�α���������Ƶ��θ�λ
		if(io)//�����Ϊ1
			value =value|0x80;//��1�ŵ����λ
		clk =1;//������һλ��ʱ�������ߣ�Ϊ�ٴγ����½�����׼��
	
	}
	ce =0;//����һ�ζ���������
	return value ;//����ȡ��1�ֽ����ݴ��ݳ�ȥ
}



/***************��ʼ��**************/
/**********ʱ�䡢���ڵĳ�ʼֵ����**********/

void ds1302_initial()
{
	uchar i,temp;
	for(i=0; i<7; i++)//���Ĵ�������ֵ֮ǰ�������ڡ�ʱ��ת��ΪBCD��
	{
		temp = tim0[i]/10;//ȡʮλ
		tim0[i]=temp*16+tim0[i]%10;//bcd ��
	}
/****************************************************/	
/************������ʾ��ʽ12/24��AM/PM*************************/
/****************************************************/
		if(tim0[7]==1)
		{
			if(tim0[8]==1)
				tim0[4]|=0xa0;//12СʱPM
			else
				tim0[4]|=0x80;//12СʱAM
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
	write_1302_byte(0x8e,0x00);//���Ĵ������ó�ʼֵ֮ǰ����Ҫȥ��д����
	
	for(i=0; i<7; i++)//ÿ��ѭ����һ���Ĵ�������ֵ
	{
		write_1302_byte(add_write[i],tim0[i]);
	}
	
	write_1302_byte(0x8e,0x80);//���Ĵ������ó�ʼֵ����Ҫ����д����
	

}



/***************��ȡʱ�䲢����ת��*************/

void read_1302time()
{
	uchar i;
	for(i=0; i<7; i++)//ÿ��ѭ����һ���Ĵ������ݣ����浽tim_bcd[]������
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
	
	for(i=0; i<7; i++)//�����������ڡ�ʱ��ֽ�Ϊʮλ�͸�λ
	{
//		distim[i*2]=(tim_bcd[i]/16)*10+tim_bcd[i]%16;
//		distim[i*2+1]=0x20;
		tim[2*i]=tim_bcd[i]/16;
		tim[2*i+1]=tim_bcd[i]%16;	
	}
}



/***************��ʾ���ں�ʱ��**************/

void display_1302time()
{
	
//��һ��
	write_com(0x80);
	write_data(0x30+tim[0]);//��
	write_data(0x30+tim[1]);
	write_data('-');
	write_data(0x30+tim[4]);//��
	write_data(0x30+tim[5]);
	write_data('-');
	write_data(0x30+tim[6]);//��
	write_data(0x30+tim[7]);
	write_data(' ');
//	write_data(0x30+tim[2]);//��
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
	
//�ڶ���
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
///*��ɵ�֧����*/
// const char *cTianGanDiZhi[] = {"����","�ҳ�","����","��î","�쳽","����","��δ",
//							"����","����","����","�Һ�","����"};

// /*��������*/
// const char *cShuXiang[] = {"��","��","��","��","��","��",
//							"��","��","��","ţ","��","��"};
// /*ũ��������*/
// const char *cDayName[] = {"*","��һ","����","����","����","����",
//						   "����","����","����","����","��ʮ",
//						   "ʮһ","ʮ��","ʮ��","ʮ��","ʮ��",
//						   "ʮ��","ʮ��","ʮ��","ʮ��","��ʮ",
//						   "إһ","إ��","إ��","إ��","إ��",       
//						   "إ��","إ��","إ��","إ��","��ʮ"}; 

// /*ũ���·���*/
// const char *cMonName[] = {"*","��","��","��","��","��","��",
//						   "��","��","��","ʮ","ʮһ","��"}; 
// 
//uchar  moon_shuju[4] = {0,0,0,0};		//�����洢ũ�����ݣ��£���
//data uchar set[7] = {0x01,0x20,0x14,0x09,0x11,0x07,0x05};//��ֵ��sec,min,hour,day,month,year,week
//code uchar day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};//�·����ݱ�1-9�·ݣ�����ũ��������Ԫ����������
//code uint day_code2[3]={0x111,0x130,0x14e};							  //10-11-12�·�
//bit c_moon;
//data uchar year_moon,month_moon,day_moon;
///*�Ӻ���,���ڶ�ȡ���ݱ���ũ���µĴ��»�С��,�������Ϊ�󷵻�1,ΪС����0*/
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

//void Conversion(bit c,uchar year,uchar month,uchar day)//c=0 Ϊ21����,c=1 Ϊ19���� ����������ݾ�ΪBCD����
//{                         
//		uchar temp1,temp2,temp3,month_p;
//		uint temp4,table_addr;
//		bit flag2,flag_y;
//		uchar Yearl,Moon,Deay;
//		temp1=year/16;   //BCD->hex �Ȱ�����ת��Ϊʮ������
//		temp2=year%16;
//		year=temp1*10+temp2;
//		temp1=month/16;
//		temp2=month%16;
//		month=temp1*10+temp2;
//		temp1=day/16;
//		temp2=day%16;
//		day=temp1*10+temp2;
//		//��λ���ݱ��ַ
//		if(c==0)
//		 {                   
//			 table_addr=(year+0x64-1)*0x3;
//		 }
//		else 
//		 {
//		  	table_addr=(year-1)*0x3;
//		 }
//		//��λ���ݱ��ַ���
//		//ȡ���괺�����ڵĹ����·�
//		temp1=year_code(table_addr+2)&0x60; 
//		temp1=_cror_(temp1,5);
//		//ȡ���괺�����ڵĹ����·����
//		//ȡ���괺�����ڵĹ�����
//		temp2=year_code(table_addr+2)&0x1f; 
//		//ȡ���괺�����ڵĹ��������
//		// ���㵱�괺���뵱��Ԫ��������,����ֻ���ڹ���1�»�2��
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
//		if ((month>0x2)&&(year%0x4==0))//��������´���2�²��Ҹ����2��Ϊ����,������1
//		 {  
//				temp4+=1;
//		 }
//		//���㹫�����뵱��Ԫ�����������
//		//�жϹ������ڴ���ǰ���Ǵ��ں�
//		if (temp4>=temp3)//�������ڴ��ں����Ǵ��ڵ���ʹ����������������
//		 { 
//				temp4-=temp3;
//				month=0x1;
//				month_p=0x1;  //month_pΪ�·�ָ��,�������ڴ���ǰ����Ǵ��ڵ���month_pָ������
//				flag2=get_moon_day(month_p,table_addr); //����ũ����Ϊ��С����С��,���·���1,С�·���0
//				flag_y=0;
//				if(flag2==0)temp1=0x1d; //С��29��
//				else temp1=0x1e; //��С30��
//				temp2=year_code(table_addr)&0xf0;
//				temp2=_cror_(temp2,4);  //�����ݱ���ȡ����������·�,��Ϊ0�����������
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
//		else  //�������ڴ���ǰʹ����������������
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
//		c_moon=c;                 //HEX->BCD ,���������,������ת��ΪBCD����
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
  KEY_1 = 1;KEY_2 = 1;KEY_3 = 1;KEY_4 = 1;//��ʼ����
		Delayms(1500);
		initial_12864();		 //12864��ʼ��
		ds1302_initial();
		while(1)
		{
			display_DHT11();
			read_1302time();
			display_1302time();
			sss();
		}
}





void Set_time(unsigned char sel)//����ѡ���������Ӧ��Ŀ��1��д��DS1302
 { 
  signed char address,item;
  signed char max,mini;
	Putstring(0x9d,"��ʱ");
  if(sel==5)  {write_data(0x30+6);address=0x82; max=59;mini=0;}    //����6
  if(sel==4)  {write_data(0x30+5);address=0x84; max=23;mini=0;}    //Сʱ5
  if(sel==3)  {write_data(0x30+4);address=0x8a; max=7;mini=1;}    //����4  
  if(sel==2)  {write_data(0x30+3);address=0x86; max=31;mini=1;}    //��3
  if(sel==1)  {write_data(0x30+2);address=0x88; max=12;mini=1;}    //��2
  if(sel==0)  {write_data(0x30+1);address=0x8c; max=99; mini=0;}    //��1
  //��ȡ1302ĳ��ַ�ϵ���ֵת����10���Ƹ���item 
  item=((read_1302_byte(address+1))/16)*10 + (read_1302_byte(address+1))%16;
		if(KEY_2 == 0)
		 {
			item++;//���� 1  
		 }
		if(KEY_4 == 0)
		 {
			item--;//���� 1 
		 }
	
  if(item>max) item=mini;//�鿴��ֵ��Ч��Χ   
  if(item<mini) item=max;           
  write_1302_byte(0x8e,0x00);//����д����
  write_1302_byte(address,(item/10)*16+item%10);//ת����16����д��1302
  write_1302_byte(0x8e,0x80);//д��������ֹд���� 
	read_1302time();

}

sss()
{

	if (w == 0)   //������ʱ
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
	else 		//������ʱ
	{
	}

	if (KEY_3 == 0) // ����ʱ��
	{
			Delayms(20);	   //ȥ��
			if(KEY_3 == 0 && w == 1)  //���ǵ�ʱ״̬ �������ڵ�����һ��
			{
					e++;
					if (e >= 6 )
					{
						e = 0;
					}
					Set_time(e);//����				
			 }
			if(KEY_3 == 0 && w == 0)  //��������״̬ʱ�ͽ����ʱ״̬
			 {
//					led_b = 1;//
					write_com(0x01);//����
					w=1;	  //�����ʱ				
					Set_time(e);//����
			 }
			 while(KEY_3 == 0);//�ȴ����ɿ� 
	}

	if(KEY_1 == 0) // ���ڵ�ʱ״̬ʱ���˳���ʱ
	{
			Delayms(20);
			if(KEY_1 == 0 && w == 1)
			{				
					w = 0;	  //�˳���ʱ
					e = 0;		//����һ���������0								
			 }
			if(KEY_1 == 0 && w == 0)
			{				
					write_com(0x01);//����
					while(KEY_1 == 0);//�ȴ����ɿ�  
			}
				write_com(0x01);//����
				read_1302time();
			while(KEY_1 == 0);//�ȴ����ɿ�  
	}

	if (KEY_2 == 0 && w == 1)// �Ӽ�����
	{
			Delayms(20);
			if(KEY_2 == 0 && w == 1)
			{				
					Set_time(e);//����
			}
			while(KEY_2 == 0);//�ȴ����ɿ�
	}

	if (KEY_4 == 0 && w == 1) // �Ӽ����� 
	{     	
			Delayms(20);	
			if(KEY_4 == 0 && w == 1)
			{				
					Set_time(e);//����
			}
			while(KEY_4 == 0);//�ȴ����ɿ� 
	}

}









