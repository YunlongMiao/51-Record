# include <reg52.h>
# define uchar unsigned char
# define uint unsigned int


//初始化0、1号外部中断
void init();

void main()
{
  init();
  while(1);  //等待中断发生
}

void init() 
{
  EA = 1;	 		//开启总中断
  EX0 = 1;	 	//开启0号外部中断
  IT0 = 1;	 	//设置外部中断触发方式 
	
  EX1 = 1;   //1号外部中断
  IT1 = 1;
}

/****0 - 低电平触发******/
/****1 - 负跳变触发******/


void ex0_intr() interrupt 0 
{ 
  P1^0=0;
}

void ex1_intr() interrupt 2 
{ 
  P1^1=0;
}
