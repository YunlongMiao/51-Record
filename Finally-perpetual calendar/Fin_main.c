#include"Fin_main.h"
#include"Fin_12864.h"
#include"Fin_dht11.h"
#include"Fin_iic.h"
#include"Fin_ds1302.h"
#include"Fin_conversion.h"
uchar e=0;
bit w = 0; //��ʱ��־λ
sss();


 void Set_time(unsigned char sel);

void main()
{
	uchar e=0,i=0,s=0;
  unsigned char c_sun,year_sun,month_sun,day_sun;    
 KEY_1 = 1;KEY_2 = 1;KEY_3 = 1;KEY_4 = 1;//��ʼ����
		Delayms(1500);
		initial_12864();		 //12864��ʼ��
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





void Set_time(unsigned char sel)//����ѡ���������Ӧ��Ŀ��1��д��DS1302
 { 
  signed char address,item;
  signed char max,mini;
	Putstring(0x9d,"��ʱ");
  if(sel==5)  {write_data(0x30+6);address=0x82; max=59;mini=0;}    //����6
  if(sel==4)  {write_data(0x30+5);address=0x84; max=12;mini=1;}    //Сʱ5
  if(sel==3)  {write_data(0x30+4);address=0x8a; max=7;mini=1;}    //����4  
  if(sel==2)  {write_data(0x30+3);address=0x86; max=31;mini=1;}    //��3
  if(sel==1)  {write_data(0x30+2);address=0x88; max=12;mini=1;}    //��2
  if(sel==0)  {write_data(0x30+1);address=0x8c; max=99; mini=0;}    //��1
  //��ȡ1302ĳ��ַ�ϵ���ֵת����10���Ƹ���item 
  item=((read_1302_byte(address+1))/16)*10 + (read_1302_byte(address+1))%16;
		if(KEY_2 == 0)
		 {
			item++;//���� 1  
		 }
		if(KEY_4 == 0)
		 {
			item--;//���� 1 
		 }
	
  if(item>max) item=mini;//�鿴��ֵ��Ч��Χ   
  if(item<mini) item=max;           
  write_1302_byte(0x8e,0x00);//����д����
  write_1302_byte(address,(item/10)*16+item%10);//ת����16����д��1302
  write_1302_byte(0x8e,0x80);//д��������ֹд���� 
	read_1302time();

}

sss()
{

	if (w == 0)   //������ʱ
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
	else 		//������ʱ
	{
	}

	if (KEY_3 == 0) // ����ʱ��
	{
			Delayms(20);	   //ȥ��
			if(KEY_3 == 0 && w == 1)  //���ǵ�ʱ״̬ �������ڵ�����һ��
			{
					e++;
					if (e >= 6 )
					{
						e = 0;
					}
					Set_time(e);//����				
			 }
			if(KEY_3 == 0 && w == 0)  //��������״̬ʱ�ͽ����ʱ״̬
			 {
//					led_b = 1;//
					write_com(0x01);//����
					w=1;	  //�����ʱ				
					Set_time(e);//����
			 }
			 while(KEY_3 == 0);//�ȴ����ɿ� 
	}

	if(KEY_1 == 0) // ���ڵ�ʱ״̬ʱ���˳���ʱ
	{
			Delayms(20);
			if(KEY_1 == 0 && w == 1)
			{				
					w = 0;	  //�˳���ʱ
					e = 0;		//����һ���������0								
			 }
			if(KEY_1 == 0 && w == 0)
			{				
					write_com(0x01);//����
					while(KEY_1 == 0);//�ȴ����ɿ�  
			}
				write_com(0x01);//����
				read_1302time();
			while(KEY_1 == 0);//�ȴ����ɿ�  
	}

	if (KEY_2 == 0 && w == 1)// �Ӽ�����
	{
			Delayms(20);
			if(KEY_2 == 0 && w == 1)
			{				
					Set_time(e);//����
			}
			while(KEY_2 == 0);//�ȴ����ɿ�
	}

	if (KEY_4 == 0 && w == 1) // �Ӽ����� 
	{     	
			Delayms(20);	
			if(KEY_4 == 0 && w == 1)
			{				
					Set_time(e);//����
			}
			while(KEY_4 == 0);//�ȴ����ɿ� 
	}

}


