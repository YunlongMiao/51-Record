#include"Fin_conversion.h"
#include"Fin_12864.h"
#include"Fin_iic.h"
/****************************************************/
/* 获取当前日期的农历及星期*/
/****************************************************/
/*==================================================*/
/*公历年对应的农历数据,每年三字节, 格式: */
/*第一字节BIT7-4 位表示闰月月份,值为0 为无闰月, */
/*BIT3-0 对应农历第1-4 月的大小*/
/*第二字节BIT7-0 对应农历第5-12 月大小, */
/*第三字节BIT7 表示农历第13 个月大小月份对应的位*/
/*为1 表示本农历月大(30 天),为0 表示小(29 天) */
/*第三字节BIT6-5 表示春节的公历月份,BIT4-0 表示春节的公历日期*/
/*类型:Byte:unsigned char Word:unsigned int */
/*本代码中Tim[0]/Tim[1]/Tim[2]分别为当前年月日数据*/
/*其中Tim[0]Bit7 表示世纪,1 为20 世纪19xx,0 为21 世纪20xx*/
/*Tim[10]为周数据/Tim[7]/Tim[8]/Tim[9]为农历数据,根据需要自行更换*/
/*==================================================*/
/*---------------------------------公历转换阴历年份数据表--------------------------------*/

/*天干地支名称*/
  uchar cTianGanDiZhi[12] = {"甲子","乙丑","丙寅","丁卯","戊辰","庚午","辛未",
							"壬申","癸酉","甲戌","乙亥","丙子"};//12-1

 /*属相名称*/
  uchar cShuXiang[] = {"龙","蛇","马","羊","猴","鸡",
							"狗","猪","鼠","牛","虎","兔"};//12-1
 /*农历日期名*/
  uchar cDayName[] = {"*","初一","初二","初三","初四","初五",
						   "初六","初七","初八","初九","初十",
						   "十一","十二","十三","十四","十五",
						   "十六","十七","十八","十九","二十",
						   "廿一","廿二","廿三","廿四","廿五",       
						   "廿六","廿七","廿八","廿九","三十"}; //31-1

 /*农历月份名*/
  uchar cMonName[] = {"*","正","二","三","四","五","六",//13-1
						   "七","八","九","十","十一","腊"};

unsigned char shuju[7];		//从1302读取的数据
//月份天数数据表
uchar code day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};
uchar code day_code2[3]={0x111,0x130,0x14e};
//月修正数据表
uchar code table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};
uchar c_moon,year_moon,month_moon,day_moon;
uchar GetMoonDay(unsigned char month_p,unsigned short table_addr)
/*读取数据表中农历月的大月或小月,如果该月大返回1,小返回0*/
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
/*--------------------------------公历转农历函数-----------------------------------------*/

void Conversion(uchar year,uchar month,uchar day)
{                         
	uchar temp1,temp2,temp3,month_p;
	uint temp4,table_addr;
	bit flag2,flag_y,c;	
  	table_addr=(year+0x64-1)*0x3;  //定位数据表地址
	temp1=year_code(table_addr+2)&0x60; //取当年春节所在的公历月份
	temp1=_cror_(temp1,5);
	temp2=year_code(table_addr+2)&0x1f; //取当年春节所在的公历日
	//取当年春节所在的公历日完成 
	//计算当年春年离当年元旦的天数,春节只会在公历1月或2月 
	if(temp1==0x1) 	{ temp3=temp2-1; }  
   	else  		{ temp3=temp2+0x1f-1; }

	// 计算当年春年离当年元旦的天数完成 
	//计算公历日离当年元旦的天数,为了减少运算,用了两个表 
	//day_code1[9],day_code2[3] 
	//如果公历月在九月或前,天数会少于0xff,用表day_code1[9], 
	//在九月后,天数大于0xff,用表day_code2[3] 
	//如输入公历日为8月10日,则公历日离元旦天数为day_code1[8-1]+10-1 
	//如输入公历日为11月10日,则公历日离元旦天数为day_code2[11-10]+10-1 
	if(month<10)	{ temp4=day_code1[month-1]+day-1; }
		else     	{ temp4=day_code2[month-10]+day-1;}

//	if ((month>0x2)&&(year%0x4==0)&&(year!=0)) //此语句造成2000年3月1日—12月31日之间
												//计算农历少算一天，应删除判断语句“&&(year!=0)”
	if ((month>0x2)&&(year%0x4==0))
		temp4+=1; //如果公历月大于2月并且该年的2月为闰月,天数加1
	//计算公历日离当年元旦的天数完成 
	 
	//判断公历日在春节前还是春节后
	if (temp4>=temp3)
	{ 
		temp4-=temp3; month=0x1;
		month_p=0x1;  //month_p为月份指向,公历日在春节前或就是春节当日month_p指向首月
		flag2=GetMoonDay(month_p,table_addr); //检查该农历月为大小还是小月,大月返回1,小月返回0
		flag_y=0;
      if(flag2==0)temp1=0x1d;   //小月29天
      	else    temp1=0x1e;    //大月30天
		temp2=year_code(table_addr)&0xf0;
		temp2=_cror_(temp2,4);  //从数据表中取该年的闰月月份,如为0则该年无闰月
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



			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
