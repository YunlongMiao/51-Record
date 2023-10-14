#include "sound.h"



void Timer0Init()		//100΢��@11.0592MHz
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x09;		//������ʽ1 Gate��1 INT0��P3^2
	TL0 = 0;		//���ö�ʱ��ֵ
	TH0 = 0;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0����ֱ�Ӽ�ʱ��INT0�øߵ�λ��ʼ������ʱ 
}


void start_sound()//��ʱ����ʼ��
{
	Trig=0;
	Echo=0;
	Timer0Init();
}

int get_Distance()//��ȡ���벢����(����ʱ��)
{
	uchar i,timeH,timeL;
	uint t,time,distance;
	
	Trig=1;
	for(i=0; i<10; i++)//��ʱ����10us������
		_nop_();
	
	Trig=0;
	Echo=1;
	
	t=0;
	while(!Echo)//�ȴ��͵�ƽ����
	{
		t++;
		_nop_();
		if(t>10000)//��ֹӲ������
			return -1;
	}
	
	t=0;
	while(Echo)//�ȴ��ߵ�ƽ����
	{
		t++;
		_nop_();
		if(t>20000)//��ֹӲ������
			return -2;
	}
	timeH = TH0;
	timeL = TL0;
	TH0 = 0;//����
	TL0 = 0;
	
	time = timeH*256+timeL;//��ȡʱ��
	if(time>12000)//4m��4/340=0.0117647058823529s
	{
		return -3;
	}
	distance = 1.08                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  *time*1.7/100;//t*340/2=t*17000*10^-6=0.017*t
	
	return distance;
}

//void display_distance()
//{
//	int distance;
//	distance = get_Distance();

//	if(distance<0)
//			LCD_Putstring(3,1,"����");
//	else
//	{
//		LCD_Setaddress(2,1);
//		LCD_write_dat(0x30+distance/100 );
//		LCD_write_dat(0x30+(distance/10)%10 );
//		LCD_write_dat(0x30+distance%10 );
//	}
//}

















