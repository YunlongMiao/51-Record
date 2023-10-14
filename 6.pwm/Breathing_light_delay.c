#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
//sbit LED1=P1^2;
void delayed(uint k)                 
{                                                   
	uint i,j;
	for(i=k; i>0; i--)
		for(j=1; j>0; j--);				//200us
}
int main()
{ 
	uint i;
	 while(1) 
	 {
		 for(i = 0; i < 200; i++)		//500hz
	   {  
			 P1 = 0xfe; 
			delayed(i);                            
			 P1 = 1; 
			delayed(200- i);
		 } 
	//	delayed(100);
		 for(i = 0; i < 200; i++) 
		{ 
			P1 = 1; 
			delayed(i);                         
			 P1 = 0xfe;
			 delayed(200 - i);
		 } 
//		delayed(100); 
	  }
}
