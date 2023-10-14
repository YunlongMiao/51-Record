#include"Fin_main.h"
#include"Fin_12864.h"
#include"Fin_dht11.h"
#include"Fin_iic.h"
#include"Fin_ds1302.h"
#include"Fin_conversion.h"
uchar e=0;
bit w = 0; //调时标志位
sss();


 void Set_time(unsigned char sel);

void main()
{
	uchar e=0,i=0,s=0;
  unsigned char c_sun,year_sun,month_sun,day_sun;    
 KEY_1 = 1;KEY_2 = 1;KEY_3 = 1;KEY_4 = 1;//初始键盘
		Delayms(1500);
		initial_12864();		 //12864初始化
//		ds1302_initial();
//	Conversion(22,3,1);
	P1=0xf;
	while(1)
		{
				Conversion(22,3,1);
//			display_DHT11();
//			read_1302time();
//			display_1302time();
//			sss();
			P1=0;
		}

}





void Set_time(unsigned char sel)//根据选择调整的相应项目加1并写入DS1302
 { 
  signed char address,item;
  signed char max,mini;
	Putstring(0x9d,"调时");
  if(sel==5)  {write_data(0x30+6);address=0x82; max=59;mini=0;}    //分钟6
  if(sel==4)  {write_data(0x30+5);address=0x84; max=12;mini=1;}    //小时5
  if(sel==3)  {write_data(0x30+4);address=0x8a; max=7;mini=1;}    //星期4  
  if(sel==2)  {write_data(0x30+3);address=0x86; max=31;mini=1;}    //日3
  if(sel==1)  {write_data(0x30+2);address=0x88; max=12;mini=1;}    //月2
  if(sel==0)  {write_data(0x30+1);address=0x8c; max=99; mini=0;}    //年1
  //读取1302某地址上的数值转换成10进制赋给item 
  item=((read_1302_byte(address+1))/16)*10 + (read_1302_byte(address+1))%16;
		if(KEY_2 == 0)
		 {
			item++;//数加 1  
		 }
		if(KEY_4 == 0)
		 {
			item--;//数减 1 
		 }
	
  if(item>max) item=mini;//查看数值有效范围   
  if(item<mini) item=max;           
  write_1302_byte(0x8e,0x00);//允许写操作
  write_1302_byte(address,(item/10)*16+item%10);//转换成16进制写入1302
  write_1302_byte(0x8e,0x80);//写保护，禁止写操作 
	read_1302time();

}

sss()
{

	if (w == 0)   //正常走时
	{
//			if(((mm/18)*10+(mm%16))%2==0) 
//			{
//					lcm_w_ss();  
//			}
//			else
//			{
//					lcm_w_nl();
//			}
	}
	else 		//启动调时
	{
	}

	if (KEY_3 == 0) // 设置时间
	{
			Delayms(20);	   //去抖
			if(KEY_3 == 0 && w == 1)  //当是调时状态 本键用于调整下一项
			{
					e++;
					if (e >= 6 )
					{
						e = 0;
					}
					Set_time(e);//调整				
			 }
			if(KEY_3 == 0 && w == 0)  //当是正常状态时就进入调时状态
			 {
//					led_b = 1;//
					write_com(0x01);//清屏
					w=1;	  //进入调时				
					Set_time(e);//调整
			 }
			 while(KEY_3 == 0);//等待键松开 
	}

	if(KEY_1 == 0) // 当在调时状态时就退出调时
	{
			Delayms(20);
			if(KEY_1 == 0 && w == 1)
			{				
					w = 0;	  //退出调时
					e = 0;		//“下一项”计数器清0								
			 }
			if(KEY_1 == 0 && w == 0)
			{				
					write_com(0x01);//清屏
					while(KEY_1 == 0);//等待键松开  
			}
				write_com(0x01);//清屏
				read_1302time();
			while(KEY_1 == 0);//等待键松开  
	}

	if (KEY_2 == 0 && w == 1)// 加减调整
	{
			Delayms(20);
			if(KEY_2 == 0 && w == 1)
			{				
					Set_time(e);//调整
			}
			while(KEY_2 == 0);//等待键松开
	}

	if (KEY_4 == 0 && w == 1) // 加减调整 
	{     	
			Delayms(20);	
			if(KEY_4 == 0 && w == 1)
			{				
					Set_time(e);//调整
			}
			while(KEY_4 == 0);//等待键松开 
	}

}


