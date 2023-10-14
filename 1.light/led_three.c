#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned int
 
void delay(uint z)//毫秒
{
	uint x,y;
	for(x = 0; x < z; x++)
		for(y = 0; y < 113; y++);
}
 

//长亮

void main() 
{
	int i;						//循环变量
	while(1)	  					
	{
		P1 = 0x01;	 			//设定LED灯初始值
		delay(700);
		for(i = 0;i < 8;i++)
		{
			P1 = (P1 << 1);		//移位、依次点亮
			
			delay(700);		//延时
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