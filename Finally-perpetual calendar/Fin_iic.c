#include"Fin_iic.h"


			
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


void response()//�������ӻ���Ӧ���ź�
{ uchar i;
sda = 1;
scl=1;
delay();
while((sda==1)&&i<250) i++; //Ӧ��sdaΪ0����Ӧ��Ϊ1
scl=0; //�ͷ�����
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
void write_add(uchar address,uchar date, uchar ss)
{  
	 uchar temp;
		if(ss)
			temp=0xa2;
		else
			temp=0xa0;
	 start();
   write_byte(temp);                    //�豸��ַ
   response();
   write_byte(address);
   response();
   write_byte(date);
   response();
		delay1(10);
	 stop();
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






