#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
sbit LED1=P1^2;
uint n=0;

void main()		//13λ��ʱ��/������
{
	TMOD=0x00;	//��ʱ��������ʽ�Ĵ���		
	TH0=144;	//(8192-3584)/32;			
	TL0=0;		//(8192-3584)%32;	//5ms
	EA=1;			////�����ж�
	ET0=1;		 //����ʱ�� 0 �ж�
	TR0=1;		 //������ʱ�� 0
	while(1)
	{
		if(n>=200)
		{
			LED1=~LED1;
			n=0;
		}
		
	}
}

void T0_timer()interrupt 1
{	
	TH0=144;		//(8192-3584)/32;			
	TL0=0;			//(8192-3584)%32;	
	n++;
}