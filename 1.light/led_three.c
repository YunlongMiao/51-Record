#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned int
 
void delay(uint z)//����
{
	uint x,y;
	for(x = 0; x < z; x++)
		for(y = 0; y < 113; y++);
}
 

//����

void main() 
{
	int i;						//ѭ������
	while(1)	  					
	{
		P1 = 0x01;	 			//�趨LED�Ƴ�ʼֵ
		delay(700);
		for(i = 0;i < 8;i++)
		{
			P1 = (P1 << 1);		//��λ�����ε���
			
			delay(700);		//��ʱ
		}
	}

}


//void main() 
//{
//	uint temp = 0xfe;
//	
//	while(1)	  					
//	{		
//		P1 = temp;
//		delay(1000);
//		temp = (temp << 1) | 0x01;
//		if(temp == 0xff)
//		{
//			temp = 0xfe;
//		}
//	}
//}