#include"iic.h"
#include"lcd12864.h"
//uchar sum[]={0,1,2,3,4,5,6,7};
//uchar num[8];
void main()
{

	
iic_write_only(0x00,0x55);//单字节写入
P0=iic_read_only(0x00);//单字节读取
	
}





