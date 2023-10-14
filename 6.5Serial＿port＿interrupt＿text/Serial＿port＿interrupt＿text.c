#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
	

void main()
{
	TMOD=0x20;		//?  ?  1      ?2-8¦Ë ?   ?
	TH1=0xe8;
	TL1=0xe8;			//   ¨°     =(  2^smod  * F)/(32*12*(256-N)=9600		//smod=0
	REN=1;
	SM0=0;				//? ?? ?  ??1
	SM1=1;
	TR1=1;				//  ?  1    
	EA=1;					// §Ø  ?   
	ES=1;					//     §Ø     ¦Ë
	while(1);
}	

void inter()interrupt 4
{
		uchar s;
		RI=0;		//    
		s=SBUF;
		RI=0;
		SBUF=s;
		while(!TI);
		TI=0;
}