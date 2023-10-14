#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned int
sbit  LED=P1^0;
uint he[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uint i;

void delayms(unsigned int s)
{
	uchar a,b,c;
	for(c=1; c>0; c--)
		for(b=38; b>0; b--)
			for(a=13; a>0; a--)
				for(; s>0; s--);
}
//sbit text1=P3^6;
//sbit text2=P3^7;

void main()
{
	
	for(i=0; i<8; i++)
	{
		P1=he[i];
	   delayms(20000);
	}
	
//	while(1)
//	{
//		text1=1;
//		delayms(1000);
//		text1=0;
//		delayms(1000);
//		text2=1;
//		delayms(1000);
//		text2=0;
//		delayms(1000);
//	}
	

	
}

