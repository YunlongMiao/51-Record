//#include"LCD1602.h"
#include"ds1302.h"
#include"lcd12864.h"
void main()
{
	initial_12864();
//	initial_1602();
	ds1302_initial();
	while(1)
	{	
		read_1302time();
		display_1302time();
		Show_number(0x98,4965);
	}

}