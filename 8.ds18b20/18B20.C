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

void Delay(int n)//��ʱ���� ����10us
{
	while(n--);
}

void Delaynms(uint x) //��ʱ
{
	uint i,j;
 	for(i=0;i<x;i++)
   		for(j=0;j<100;j++);
}


void TX(uchar TX_char)//�жϷ���
{
  SBUF=TX_char;
  while(!TI);
  TI=0;

}
	
void DS18B20_Init(void)//��ʼ��ds1820
{
	uchar x=0;
	DQ = 1;    
	Delay(8);  //��ʱ
	DQ = 0;    
	Delay(80); //��ʱ ���� 480us
	DQ = 1;    
	Delay(5);
	x=DQ;      //��ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	Delay(50);

}

uchar ReadOneChar(void)//��һ���ֽ�
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


void WriteOneChar(uchar dat)//дһ���ֽ�
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


void ReadTemperature(void)//��ȡ�¶�
{
	int a=0;
	uchar b=0;
	DS18B20_Init();			//��λ
	WriteOneChar(0xCC); // ����ROM
	WriteOneChar(0x44); // �¶�ת��
	DS18B20_Init();
	WriteOneChar(0xCC); //����ROM
	WriteOneChar(0xBE); //��ȡ�¶ȼĴ���
//	Delay(5000);
	a=ReadOneChar();  //����8λ
	b=ReadOneChar(); //����8λ
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

void Config_Ds18b20(uchar resolution)//ds18b20��������
{
	DS18B20_Init();			//��λ
	WriteOneChar(0xCC); // ����ROM
	WriteOneChar(0x4e); // д�Ĵ���
	WriteOneChar(0x7d0); // ������ֵ125
	WriteOneChar(0x00); // ������ֵ0
	WriteOneChar(resolution); //ds18b20��������
}






void Delayms(uint n)		//��ʱʱ��Ϊ 1ms * x ������11.0592M
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
                dat=dat<<1;         //����һλ         
         }
}



//д������
void LCD_write_com(uchar com)
{        
           LCD_sendbyte(0xf8);          //����5�������ġ�1��������һ������,11111,RW(0),RS(0),0
           LCD_sendbyte(0xf0&com);      //ȡ����λ�����ݷ����δ��ͣ�ÿ���ֽڵ����ݱ����������ֽ�,����λ���ڵ�һ���ֽڵĸ���λ                                 
          LCD_sendbyte(0xf0&(com<<4));  //����λ���ڵڶ����ֽڵĸ���λ           
          Delayms(1);                   //���в�֧�ֶ����������ɼ��æ��������������ʱ���
} 



//д������
void LCD_write_dat(uchar dat)		
{      
           LCD_sendbyte(0xfa);        
           LCD_sendbyte(0xf0&dat);      
           LCD_sendbyte(0xf0&(dat<<4));            
           Delayms(1);  
}



//LCD��ʼ��
void LCD_init(void)
{        
         LCD_write_com(0x30);    //ѡ�����ָ�            
        LCD_write_com(0x0c);    //����ʾ�����α꣬������   
         LCD_write_com(0x01);    //�����ʾ��Ļ����DDRAMλַ����������Ϊ00H  
         Delayms(1);            //��������ʱ��ϳ�1.6ms ��˼Ӵ���ʱ 
         LCD_write_com(0x02);    //��DDRAMλַ��λ,�˴�ò���������ظ�  
        LCD_write_com(0x06);    //�趨������ƣ�������ʾ���ƶ�  
}


//LCD��Ļ��ַ��λ
void LCD_Setaddress(uchar x,uchar y) //��ַ�ӵ�1�е�1�п�ʼ����0��ʼ��y��
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
           LCD_write_com(addr);   //�ַ���ʾ��ʼ��ַ
}




//LCD��ʾ�ַ����飨���֣�
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
	Config_Ds18b20(0x7f);//ds18b20��������	0x1f-9 0x3f-10 0x5f-11 0x7f-12
  Int_return();//���ڳ�ʼ��
	LCD_init();
	LCD_Putstring(1,0,"�¶�");
	while(1)
	{  
			ReadTemperature();	 //��ȡ�¶�
//		TX(Tem[0]);
//		TX(Tem[1]+0x30);
//		TX(Tem[2]+0x30);			//��
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