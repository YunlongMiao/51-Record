#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned int
 
void delay(uint z)//ºÁÃë
{
	uint x,y;
	for(x = 0; x < z; x++)
		for(y = 0; y < 113; y++);
}
void main()
{
		P1 = 0XFE;
		delay(1000);
		P1 = 0XFD;
		delay(1000);
		P1 = 0XFB;
		delay(1000);
		P1 = 0XF7;
		delay(1000);
		P1 = 0XEF;
		delay(1000);
		P1 = 0XDF;
		delay(1000);
		P1 = 0XBF;
		delay(1000);
		P1 = 0X7F;
		delay(1000);
}