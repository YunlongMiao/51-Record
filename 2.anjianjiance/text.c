#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned int

sbit btn = P3^3;
uint he[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
char flag = 0;

void delay(uint z)//毫秒
{
	uint x,y;
	for(x = 0; x < z; x++)
		for(y = 0; y < 113; y++);
}

void text()
{
				if (btn == 0)
        {
            delay(10);  //消抖while(!btn)					
            if (btn == 0)
            {
                flag = !flag;
            }
        }
}


//void main(void)
//{
//    char flag = 0;
//    unsigned char tmp = 0xFF;
//    while (1)
//    {
//        btn = 1;
//        delay(10); //大约10ms
//        if (btn == 0)
//        {
//            delay(10);  //消抖
//            if (btn == 0)
//            {
//                flag = 1;
//            }
//        }
//         
//        if (flag)
//        {
//            P1 = tmp;
//            tmp >>= 1;
//            delay(300);
//            flag = 0;
//					if()
//        }
//    }    
//}

void main(void)
{
    char i;
    
    while (1)
    {
        if(flag==1)
					for(i=0; i<8; i++)
					{
						P1=he[i];
						delay(100);
						text();
						if(flag!=1)
							break;
					}
				
				 if(flag==0)
					for(i=7; i>0; i--)
					{
						P1=he[i];
						delay(100);
						text();
						if(flag!=0)
							break;
					}
    }    
}