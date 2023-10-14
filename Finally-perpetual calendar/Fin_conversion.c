#include"Fin_conversion.h"
#include"Fin_12864.h"
#include"Fin_iic.h"
/****************************************************/
/* ��ȡ��ǰ���ڵ�ũ��������*/
/****************************************************/
/*==================================================*/
/*�������Ӧ��ũ������,ÿ�����ֽ�, ��ʽ: */
/*��һ�ֽ�BIT7-4 λ��ʾ�����·�,ֵΪ0 Ϊ������, */
/*BIT3-0 ��Ӧũ����1-4 �µĴ�С*/
/*�ڶ��ֽ�BIT7-0 ��Ӧũ����5-12 �´�С, */
/*�����ֽ�BIT7 ��ʾũ����13 ���´�С�·ݶ�Ӧ��λ*/
/*Ϊ1 ��ʾ��ũ���´�(30 ��),Ϊ0 ��ʾС(29 ��) */
/*�����ֽ�BIT6-5 ��ʾ���ڵĹ����·�,BIT4-0 ��ʾ���ڵĹ�������*/
/*����:Byte:unsigned char Word:unsigned int */
/*��������Tim[0]/Tim[1]/Tim[2]�ֱ�Ϊ��ǰ����������*/
/*����Tim[0]Bit7 ��ʾ����,1 Ϊ20 ����19xx,0 Ϊ21 ����20xx*/
/*Tim[10]Ϊ������/Tim[7]/Tim[8]/Tim[9]Ϊũ������,������Ҫ���и���*/
/*==================================================*/
/*---------------------------------����ת������������ݱ�--------------------------------*/

/*��ɵ�֧����*/
  uchar cTianGanDiZhi[12] = {"����","�ҳ�","����","��î","�쳽","����","��δ",
							"����","����","����","�Һ�","����"};//12-1

 /*��������*/
  uchar cShuXiang[] = {"��","��","��","��","��","��",
							"��","��","��","ţ","��","��"};//12-1
 /*ũ��������*/
  uchar cDayName[] = {"*","��һ","����","����","����","����",
						   "����","����","����","����","��ʮ",
						   "ʮһ","ʮ��","ʮ��","ʮ��","ʮ��",
						   "ʮ��","ʮ��","ʮ��","ʮ��","��ʮ",
						   "إһ","إ��","إ��","إ��","إ��",       
						   "إ��","إ��","إ��","إ��","��ʮ"}; //31-1

 /*ũ���·���*/
  uchar cMonName[] = {"*","��","��","��","��","��","��",//13-1
						   "��","��","��","ʮ","ʮһ","��"};

unsigned char shuju[7];		//��1302��ȡ������
//�·��������ݱ�
uchar code day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};
uchar code day_code2[3]={0x111,0x130,0x14e};
//���������ݱ�
uchar code table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};
uchar c_moon,year_moon,month_moon,day_moon;
uchar GetMoonDay(unsigned char month_p,unsigned short table_addr)
/*��ȡ���ݱ���ũ���µĴ��»�С��,������´󷵻�1,С����0*/
{
	switch (month_p)
	{
		case 1:
			if((year_code(table_addr)&0x08)==0)		return(0);
			else 									return(1); 
		case 2:
			if((year_code(table_addr)&0x04)==0)		return(0);
			else 									return(1);
		case 3:
			if((year_code(table_addr)&0x02)==0)		return(0);
			else 									return(1);
		case 4:
			if((year_code(table_addr)&0x01)==0)		return(0);
			else 								    return(1);
		case 5:
			if((year_code(table_addr)&0x80)==0)	return(0);
			else 									return(1);
		case 6:
			if((year_code(table_addr)&0x40)==0)	return(0);
			else 									return(1);
		case 7:
			if((year_code(table_addr+1)&0x20)==0)	return(0);
			else 									return(1);
		case 8:
			if((year_code(table_addr+1)&0x10)==0)	return(0);
			else 									return(1);
		case 9:
			if((year_code(table_addr+1)&0x08)==0)	return(0);
			else 									return(1);
		case 10:
			if((year_code(table_addr+1)&0x04)==0)	return(0);
			else 									return(1);
		case 11:
			if((year_code(table_addr+1)&0x02)==0)	return(0);
			else 									return(1);
		case 12:
			if((year_code(table_addr+1)&0x01)==0)	return(0);
			else 									return(1);
		case 13:
			if((year_code(table_addr+2)&0x80)==0)	return(0);
			else 									return(1);
	}
	return(0);
}
/*--------------------------------����תũ������-----------------------------------------*/

void Conversion(uchar year,uchar month,uchar day)
{                         
	uchar temp1,temp2,temp3,month_p;
	uint temp4,table_addr;
	bit flag2,flag_y,c;	
  	table_addr=(year+0x64-1)*0x3;  //��λ���ݱ��ַ
	temp1=year_code(table_addr+2)&0x60; //ȡ���괺�����ڵĹ����·�
	temp1=_cror_(temp1,5);
	temp2=year_code(table_addr+2)&0x1f; //ȡ���괺�����ڵĹ�����
	//ȡ���괺�����ڵĹ�������� 
	//���㵱�괺���뵱��Ԫ��������,����ֻ���ڹ���1�»�2�� 
	if(temp1==0x1) 	{ temp3=temp2-1; }  
   	else  		{ temp3=temp2+0x1f-1; }

	// ���㵱�괺���뵱��Ԫ����������� 
	//���㹫�����뵱��Ԫ��������,Ϊ�˼�������,���������� 
	//day_code1[9],day_code2[3] 
	//����������ھ��»�ǰ,����������0xff,�ñ�day_code1[9], 
	//�ھ��º�,��������0xff,�ñ�day_code2[3] 
	//�����빫����Ϊ8��10��,��������Ԫ������Ϊday_code1[8-1]+10-1 
	//�����빫����Ϊ11��10��,��������Ԫ������Ϊday_code2[11-10]+10-1 
	if(month<10)	{ temp4=day_code1[month-1]+day-1; }
		else     	{ temp4=day_code2[month-10]+day-1;}

//	if ((month>0x2)&&(year%0x4==0)&&(year!=0)) //��������2000��3��1�ա�12��31��֮��
												//����ũ������һ�죬Ӧɾ���ж���䡰&&(year!=0)��
	if ((month>0x2)&&(year%0x4==0))
		temp4+=1; //��������´���2�²��Ҹ����2��Ϊ����,������1
	//���㹫�����뵱��Ԫ����������� 
	 
	//�жϹ������ڴ���ǰ���Ǵ��ں�
	if (temp4>=temp3)
	{ 
		temp4-=temp3; month=0x1;
		month_p=0x1;  //month_pΪ�·�ָ��,�������ڴ���ǰ����Ǵ��ڵ���month_pָ������
		flag2=GetMoonDay(month_p,table_addr); //����ũ����Ϊ��С����С��,���·���1,С�·���0
		flag_y=0;
      if(flag2==0)temp1=0x1d;   //С��29��
      	else    temp1=0x1e;    //����30��
		temp2=year_code(table_addr)&0xf0;
		temp2=_cror_(temp2,4);  //�����ݱ���ȡ����������·�,��Ϊ0�����������
		while(temp4>=temp1)
		{
			temp4-=temp1; month_p+=1;
			if(month==temp2)
	    	{
				flag_y=~flag_y;
         	if(flag_y==0)month+=1;
			}
			else month+=1;
			flag2=GetMoonDay(month_p,table_addr);
         if(flag2==0)temp1=0x1d;
         	else    temp1=0x1e;
		}
		day=temp4+1;
	}
	else
	{  
		temp3-=temp4;
		if (year==0x0){year=0x63;c=1;}
			else	year-=1;
		table_addr-=0x3;
		month=0xc;
		temp2=year_code(table_addr)&0xf0;
		temp2=_cror_(temp2,4);
		if (temp2==0)month_p=0xc; 
			else	month_p=0xd; 
		flag_y=0;
		flag2=GetMoonDay(month_p,table_addr);
		if(flag2==0)temp1=0x1d;
      	else	temp1=0x1e;
		while(temp3>temp1)
		{
			temp3-=temp1;
			month_p-=1;
			if(flag_y==0)month-=1;
         if(month==temp2)flag_y=~flag_y;
         flag2=GetMoonDay(month_p,table_addr);
         if(flag2==0)temp1=0x1d;
            else	temp1=0x1e;
		}
		day=temp1-temp3+1;
	}
	
		c_moon=c;
		year_moon=year;
		month_moon=month;
		day_moon=day;
//			Show_number(0x88,year_moon);
//			Show_number(0x8a,month_moon);
//			Show_number(0x8c,day_moon);
//			Putstring(0x90,cTianGanDiZhi[year_moon%60-8]);
			Putstring(0x92,cShuXiang[year_moon%12]);
			Putstring(0x94,cMonName[month_moon]);
			Putstring(0x96,cDayName[day_moon]);
}



			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
