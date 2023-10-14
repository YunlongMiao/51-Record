#include<reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
Unsigned char code dis_week[]={"SUN,MON,TUE,WED,THU,FRI,SAT"};
unsigned char data dis_buf1[16];
unsigned char data dis_buf2[16]; 
sbit	 rs = P3^0;				//LCD数据/命令选择端(H/L)
sbit	 rw = P3^1;				//LCD读/写选择端(H/L)
sbit	 ep = P2^2;				//LCD使能控制
void delayms(unsigned char ms);	//延时程序
bit  lcd_busy();				//测试LCD忙碌状态程序
void lcd_wcmd(char cmd);		//写入指令到LCD程序
void lcd_wdat(char dat);		//写入数据到LCD程序
void lcd_pos(char pos);			//LCD数据指针位置程序
void lcd_init();				//LCD初始化设定程序

void pro_display();				//显示处理程序

void update_disbuf();			//更新显示缓冲区函数
unsigned char Conver_week(bit c,uchar year,uchar month,uchar day);//调用week函数让下面函数能够调用 
bit c_moon;
data uchar year_moon,month_moon,day_moon,week;
uchar c_sun,year_sun,month_sun,day_sun;

// 延时程序
void delay(unsigned char ms)
{	while(ms--)
	{	unsigned char i;
		for(i = 0; i< 250; i++)     
		{
			_nop_();			   //执行一条_nop_()指令为一个机器周期
			_nop_();
			_nop_();
			_nop_();
		}
	}
}		
	

//测试LCD忙碌状态
bit lcd_busy()
{	
	bit result;
	rs = 0;
	rw = 1;
	ep = 1;
	_nop_();

	result =(bit)(P0&0x80);	//LCD的D0--D7中,D7=1为忙碌,D7=0为空闲
	ep = 0;
	return result;	
}

//写入指令到LCD
void lcd_wcmd(char cmd)
{							
	while(lcd_busy());	//当lcd_busy为1时,再次检测LCD忙碌状态,lcd-busy为0时,开始写指令
	rs = 0;
	rw = 0;
	ep = 0;
	_nop_();	
	P0 = cmd;
	_nop_();
	ep = 1;
	_nop_();
	ep = 0;		
}

//写入数据到LCD
void lcd_wdat(char dat)	
{							
	while(lcd_busy());	//当lcd_busy为1时,再次检测LCD忙碌状态,lcd-busy为0时,开始写数据
	rs = 1;
	rw = 0;
	ep = 0;
	P0 = dat;
	_nop_();
	ep = 1;
	_nop_();
	_nop_();
	ep = 0;	
}

//LCD数据指针位置程序
void lcd_pos(char pos)
{						
	lcd_wcmd(pos|0x80);	//数据指针=80+地址码(00H~27H,40H~67H)
}
//LCD初始化设定
void lcd_init()
{						
	lcd_wcmd(0x38);		//设置LCD为16X2显示,5X7点阵,八位数据借口
	delay(1);
	lcd_wcmd(0x0c);		//LCD开显示及光标设置(光标不闪烁,不显示"-")
	delay(1);
	lcd_wcmd(0x06);		//LCD显示光标移动设置(光标地址指针加1,整屏显示不移动)
	delay(1);
	lcd_wcmd(0x01);		//清除LCD的显示内容
	delay(1);
}
//更新显示缓冲区


void update_disbuf()	
{	 
	dis_buf1[0]=0x59;               //Y
	dis_buf1[1]=0x20;
	if(c_sun==0)
	{
	dis_buf1[2]=50; 			//'2' 
	dis_buf1[3]=48;             //'0'
	}
	else
	{
	  dis_buf1[2]=49; 			//'1' 
	  dis_buf1[3]=57;           //'9'
	}
    dis_buf1[4]=year_sun/16+48; //48是在1602上能显示对应的数字
	dis_buf1[5]=year_sun%16+48; 
	dis_buf1[6]=0x2d;
	dis_buf1[7]=month_sun/16+48;
	dis_buf1[8]=month_sun%16+48; 
	dis_buf1[9]=0x2d; 			//'-'
	dis_buf1[10]=day_sun/16+48;
	dis_buf1[11]=day_sun%16+48;
	dis_buf1[12]=0x20;
	dis_buf1[13]=dis_week[4*week];
	dis_buf1[14]=dis_week[4*week+1];
	dis_buf1[15]=dis_week[4*week+2];

	dis_buf2[0]=0x4e; 			// N
	dis_buf2[1]=0x20; 			//空格
	if(c_sun==0)
	{
	dis_buf2[2]=50; 			//'2' 
	dis_buf2[3]=48;             //'0'
	}
	else
	{
	  dis_buf2[2]=49; 			//'1' 
	  dis_buf2[3]=57;           //'9'
	}
	dis_buf2[4]=year_moon/10+48; 
	dis_buf2[5]=year_moon%10+48; 
	dis_buf2[6]=0x2d;
	dis_buf2[7]=month_moon/10+48;
	dis_buf2[8]=month_moon%10+48; 
	dis_buf2[9]=0x2d; 			//'-'
	dis_buf2[10]=day_moon/10+48;
	dis_buf2[11]=day_moon%10+48;


}
//显示处理程序
void pro_display()
{	unsigned char i;
	lcd_pos(0x00);
	for (i=0;i<=15;i++)
		{lcd_wdat(dis_buf1[i]);}
	lcd_pos(0x40);
   	for (i=0;i<=11;i++)
		{lcd_wdat(dis_buf2[i]);}

}
code uchar year_code[567]={
                    0x04,0xAe,0x53,    //1901 0
                    0x0A,0x57,0x48,    //1902 3
                    0x55,0x26,0xBd,    //1903 6
                    0x0d,0x26,0x50,    //1904 9
                    0x0d,0x95,0x44,    //1905 12
                    0x46,0xAA,0xB9,    //1906 15
                    0x05,0x6A,0x4d,    //1907 18
                    0x09,0xAd,0x42,    //1908 21
                    0x24,0xAe,0xB6,    //1909
                    0x04,0xAe,0x4A,    //1910
                    0x6A,0x4d,0xBe,    //1911
                    0x0A,0x4d,0x52,    //1912
                    0x0d,0x25,0x46,    //1913
                    0x5d,0x52,0xBA,    //1914
                    0x0B,0x54,0x4e,    //1915
                    0x0d,0x6A,0x43,    //1916
                    0x29,0x6d,0x37,    //1917
                    0x09,0x5B,0x4B,    //1918
                    0x74,0x9B,0xC1,    //1919
                    0x04,0x97,0x54,    //1920
                    0x0A,0x4B,0x48,    //1921
                    0x5B,0x25,0xBC,    //1922
                    0x06,0xA5,0x50,    //1923
                    0x06,0xd4,0x45,    //1924
                    0x4A,0xdA,0xB8,    //1925
                    0x02,0xB6,0x4d,    //1926
                    0x09,0x57,0x42,    //1927
                    0x24,0x97,0xB7,    //1928
                    0x04,0x97,0x4A,    //1929
                    0x66,0x4B,0x3e,    //1930
                    0x0d,0x4A,0x51,    //1931
                    0x0e,0xA5,0x46,    //1932
                    0x56,0xd4,0xBA,    //1933
                    0x05,0xAd,0x4e,    //1934
                    0x02,0xB6,0x44,    //1935
                    0x39,0x37,0x38,    //1936
                    0x09,0x2e,0x4B,    //1937
                    0x7C,0x96,0xBf,    //1938
                    0x0C,0x95,0x53,    //1939
                    0x0d,0x4A,0x48,    //1940
                    0x6d,0xA5,0x3B,    //1941
                    0x0B,0x55,0x4f,    //1942
                    0x05,0x6A,0x45,    //1943
                    0x4A,0xAd,0xB9,    //1944
                    0x02,0x5d,0x4d,    //1945
                    0x09,0x2d,0x42,    //1946
                    0x2C,0x95,0xB6,    //1947
                    0x0A,0x95,0x4A,    //1948
                    0x7B,0x4A,0xBd,    //1949
                    0x06,0xCA,0x51,    //1950
                    0x0B,0x55,0x46,    //1951
                    0x55,0x5A,0xBB,    //1952
                    0x04,0xdA,0x4e,    //1953
                    0x0A,0x5B,0x43,    //1954
                    0x35,0x2B,0xB8,    //1955
                    0x05,0x2B,0x4C,    //1956
                    0x8A,0x95,0x3f,    //1957
                    0x0e,0x95,0x52,    //1958
                    0x06,0xAA,0x48,    //1959
                    0x7A,0xd5,0x3C,    //1960
                    0x0A,0xB5,0x4f,    //1961
                    0x04,0xB6,0x45,    //1962
                    0x4A,0x57,0x39,    //1963
                    0x0A,0x57,0x4d,    //1964
                    0x05,0x26,0x42,    //1965
                    0x3e,0x93,0x35,    //1966
                    0x0d,0x95,0x49,    //1967
                    0x75,0xAA,0xBe,    //1968
                    0x05,0x6A,0x51,    //1969
                    0x09,0x6d,0x46,    //1970
                    0x54,0xAe,0xBB,    //1971
                    0x04,0xAd,0x4f,    //1972
                    0x0A,0x4d,0x43,    //1973
                    0x4d,0x26,0xB7,    //1974
                    0x0d,0x25,0x4B,    //1975
                    0x8d,0x52,0xBf,    //1976
                    0x0B,0x54,0x52,    //1977
                    0x0B,0x6A,0x47,    //1978
                    0x69,0x6d,0x3C,    //1979
                    0x09,0x5B,0x50,    //1980
                    0x04,0x9B,0x45,    //1981
                    0x4A,0x4B,0xB9,    //1982
                    0x0A,0x4B,0x4d,    //1983
                    0xAB,0x25,0xC2,    //1984
                    0x06,0xA5,0x54,    //1985
                    0x06,0xd4,0x49,    //1986
                    0x6A,0xdA,0x3d,    //1987
                    0x0A,0xB6,0x51,    //1988
                    0x09,0x37,0x46,    //1989
                    0x54,0x97,0xBB,    //1990
                    0x04,0x97,0x4f,    //1991
                    0x06,0x4B,0x44,    //1992
                    0x36,0xA5,0x37,    //1993
                    0x0e,0xA5,0x4A,    //1994
                    0x86,0xB2,0xBf,    //1995
                    0x05,0xAC,0x53,    //1996
                    0x0A,0xB6,0x47,    //1997
                    0x59,0x36,0xBC,    //1998
                    0x09,0x2e,0x50,    //1999 294
                    0x0C,0x96,0x45,    //2000 297
                    0x4d,0x4A,0xB8,    //2001
                    0x0d,0x4A,0x4C,    //2002
                    0x0d,0xA5,0x41,    //2003
                    0x25,0xAA,0xB6,    //2004
                    0x05,0x6A,0x49,    //2005
                    0x7A,0xAd,0xBd,    //2006
                    0x02,0x5d,0x52,    //2007
                    0x09,0x2d,0x47,    //2008
                    0x5C,0x95,0xBA,    //2009
                    0x0A,0x95,0x4e,    //2010
                    0x0B,0x4A,0x43,    //2011
                    0x4B,0x55,0x37,    //2012
                    0x0A,0xd5,0x4A,    //2013
                    0x95,0x5A,0xBf,    //2014
                    0x04,0xBA,0x53,    //2015
                    0x0A,0x5B,0x48,    //2016
                    0x65,0x2B,0xBC,    //2017
                    0x05,0x2B,0x50,    //2018
                    0x0A,0x93,0x45,    //2019
                    0x47,0x4A,0xB9,    //2020
                    0x06,0xAA,0x4C,    //2021
                    0x0A,0xd5,0x41,    //2022
                    0x24,0xdA,0xB6,    //2023
                    0x04,0xB6,0x4A,    //2024
                    0x69,0x57,0x3d,    //2025
                    0x0A,0x4e,0x51,    //2026
                    0x0d,0x26,0x46,    //2027
                    0x5e,0x93,0x3A,    //2028
                    0x0d,0x53,0x4d,    //2029
                    0x05,0xAA,0x43,    //2030
                    0x36,0xB5,0x37,    //2031
                    0x09,0x6d,0x4B,    //2032
                    0xB4,0xAe,0xBf,    //2033
                    0x04,0xAd,0x53,    //2034
                    0x0A,0x4d,0x48,    //2035
                    0x6d,0x25,0xBC,    //2036
                    0x0d,0x25,0x4f,    //2037
                    0x0d,0x52,0x44,    //2038
                    0x5d,0xAA,0x38,    //2039
                    0x0B,0x5A,0x4C,    //2040
                    0x05,0x6d,0x41,    //2041
                    0x24,0xAd,0xB6,    //2042
                    0x04,0x9B,0x4A,    //2043
                    0x7A,0x4B,0xBe,    //2044
                    0x0A,0x4B,0x51,    //2045
                    0x0A,0xA5,0x46,    //2046
                    0x5B,0x52,0xBA,    //2047
                    0x06,0xd2,0x4e,    //2048
                    0x0A,0xdA,0x42,    //2049
                    0x35,0x5B,0x37,    //2050
                    0x09,0x37,0x4B,    //2051
                    0x84,0x97,0xC1,    //2052
                    0x04,0x97,0x53,    //2053
                    0x06,0x4B,0x48,    //2054
                    0x66,0xA5,0x3C,    //2055
                    0x0e,0xA5,0x4f,    //2056
                    0x06,0xB2,0x44,    //2057
                    0x4A,0xB6,0x38,    //2058
                    0x0A,0xAe,0x4C,    //2059
                    0x09,0x2e,0x42,    //2060
                    0x3C,0x97,0x35,    //2061
                    0x0C,0x96,0x49,    //2062
                    0x7d,0x4A,0xBd,    //2063
                    0x0d,0x4A,0x51,    //2064
                    0x0d,0xA5,0x45,    //2065
                    0x55,0xAA,0xBA,    //2066
                    0x05,0x6A,0x4e,    //2067
                    0x0A,0x6d,0x43,    //2068
                    0x45,0x2e,0xB7,    //2069
                    0x05,0x2d,0x4B,    //2070
                    0x8A,0x95,0xBf,    //2071
                    0x0A,0x95,0x53,    //2072
                    0x0B,0x4A,0x47,    //2073
                    0x6B,0x55,0x3B,    //2074
                    0x0A,0xd5,0x4f,    //2075
                    0x05,0x5A,0x45,    //2076
                    0x4A,0x5d,0x38,    //2077
                    0x0A,0x5B,0x4C,    //2078
                    0x05,0x2B,0x42,    //2079
                    0x3A,0x93,0xB6,    //2080
                    0x06,0x93,0x49,    //2081
                    0x77,0x29,0xBd,    //2082
                    0x06,0xAA,0x51,    //2083
                    0x0A,0xd5,0x46,    //2084
                    0x54,0xdA,0xBA,    //2085
                    0x04,0xB6,0x4e,    //2086
                    0x0A,0x57,0x43,    //2087
                    0x45,0x27,0x38,    //2088
                    0x0d,0x26,0x4A,    //2089
                 
};
///月份数据表
code uchar day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};
code uint day_code2[3]={0x111,0x130,0x14e};

/*子函数,用于读取数据表中农历月的大月或小月,如果该月为大返回1,为小返回0*/
bit get_moon_day(uchar month_p,uint table_addr)
{
uchar temp;
    switch (month_p){
        case 1:{temp=year_code[table_addr]&0x08;
             if (temp==0)return(0);else return(1);}
        case 2:{temp=year_code[table_addr]&0x04;
             if (temp==0)return(0);else return(1);}
        case 3:{temp=year_code[table_addr]&0x02;
             if (temp==0)return(0);else return(1);}
        case 4:{temp=year_code[table_addr]&0x01;
             if (temp==0)return(0);else return(1);}
        case 5:{temp=year_code[table_addr+1]&0x80;
             if (temp==0) return(0);else return(1);}
        case 6:{temp=year_code[table_addr+1]&0x40;
             if (temp==0)return(0);else return(1);}
        case 7:{temp=year_code[table_addr+1]&0x20;
             if (temp==0)return(0);else return(1);}
        case 8:{temp=year_code[table_addr+1]&0x10;
             if (temp==0)return(0);else return(1);}
        case 9:{temp=year_code[table_addr+1]&0x08;
             if (temp==0)return(0);else return(1);}
        case 10:{temp=year_code[table_addr+1]&0x04;
             if (temp==0)return(0);else return(1);}
        case 11:{temp=year_code[table_addr+1]&0x02;
             if (temp==0)return(0);else return(1);}
        case 12:{temp=year_code[table_addr+1]&0x01;
             if (temp==0)return(0);else return(1);}
        case 13:{temp=year_code[table_addr+2]&0x80;
             if (temp==0)return(0);else return(1);}
    }
}


void Conversion(bit c,uchar year,uchar month,uchar day)
{                         //c=0 为21世纪,c=1 为19世纪 输入输出数据均为BCD数据
    uchar temp1,temp2,temp3,month_p;
    uint temp4,table_addr;
    bit flag2,flag_y;
    temp1=year/16;   //BCD->hex 先把数据转换为十六进制
    temp2=year%16;
    year=temp1*10+temp2;
    temp1=month/16;
    temp2=month%16;
    month=temp1*10+temp2;
    temp1=day/16;
    temp2=day%16;
    day=temp1*10+temp2;

    //定位数据表地址
    if(c==0){                   
        table_addr=(year+0x64-1)*0x3;
    }
    else {
        table_addr=(year-1)*0x3;
    }
    //定位数据表地址完成

    //取当年春节所在的公历月份
    temp1=year_code[table_addr+2]&0x60; 
    temp1=_cror_(temp1,5);
    //取当年春节所在的公历月份完成

    //取当年春节所在的公历日
    temp2=year_code[table_addr+2]&0x1f; 
    //取当年春节所在的公历日完成

    // 计算当年春年离当年元旦的天数,春节只会在公历1月或2月
    if(temp1==0x1){  
        temp3=temp2-1;  
    }  
    else{
        temp3=temp2+0x1f-1;        
    }
    
    if (month<10){ 
        temp4=day_code1[month-1]+day-1;
    }
    else{
        temp4=day_code2[month-10]+day-1;
    }
    if ((month>0x2)&&(year%0x4==0)){  //如果公历月大于2月并且该年的2月为闰月,天数加1
        temp4+=1;
    }
    //计算公历日离当年元旦的天数完成

    //判断公历日在春节前还是春节后
    if (temp4>=temp3){ //公历日在春节后或就是春节当日使用下面代码进行运算
        temp4-=temp3;
        month=0x1;
        month_p=0x1;  //month_p为月份指向,公历日在春节前或就是春节当日month_p指向首月
        flag2=get_moon_day(month_p,table_addr); //检查该农历月为大小还是小月,大月返回1,小月返回0
        flag_y=0;
        if(flag2==0)temp1=0x1d; //小月29天
        else temp1=0x1e; //大小30天
        temp2=year_code[table_addr]&0xf0;
        temp2=_cror_(temp2,4);  //从数据表中取该年的闰月月份,如为0则该年无闰月
        while(temp4>=temp1){
            temp4-=temp1;
            month_p+=1;
            if(month==temp2){
            flag_y=~flag_y;
            if(flag_y==0)month+=1;
            }
            else month+=1;
            flag2=get_moon_day(month_p,table_addr);
            if(flag2==0)temp1=0x1d;
            else temp1=0x1e;
        }
        day=temp4+1;
    }
    else{  //公历日在春节前使用下面代码进行运算
        temp3-=temp4;
        if (year==0x0){year=0x63;c=1;}
        else year-=1;
        table_addr-=0x3;
        month=0xc;
        temp2=year_code[table_addr]&0xf0;
        temp2=_cror_(temp2,4);
        if (temp2==0)month_p=0xc; 
        else month_p=0xd; //
        /*
        month_p为月份指向,如果当年有闰月,一年有十三个月,月指向13,
无闰月指向12
        */
        flag_y=0;
        flag2=get_moon_day(month_p,table_addr);
        if(flag2==0)temp1=0x1d;
        else temp1=0x1e;
        while(temp3>temp1){
            temp3-=temp1;
            month_p-=1;
            if(flag_y==0)month-=1;
            if(month==temp2)flag_y=~flag_y;
            flag2=get_moon_day(month_p,table_addr);
            if(flag2==0)temp1=0x1d;
            else temp1=0x1e;
         }
        day=temp1-temp3+1;
    }
    c_moon=c;                 //HEX->BCD ,运算结束后,把数据转换为BCD数据
    
	year_moon=year;
    month_moon=month;
	day_moon=day;
    
}


code uchar table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表

unsigned char Conver_week(bit c,uchar year,uchar month,uchar day)
{//c=0 为21世纪,c=1 为19世纪 输入输出数据均为BCD数据
    uchar temp1,temp2;
    temp1=year/16;   //BCD->hex 先把数据转换为十六进制
    temp2=year%16;
    year=temp1*10+temp2;
    temp1=month/16;
    temp2=month%16;
    month=temp1*10+temp2;
    temp1=day/16;
    temp2=day%16;
    day=temp1*10+temp2;
    if (c==0){year+=0x64;}  //如果为21世纪,年份数加100
    temp1=year/0x4;  //所过闰年数只算1900年之后的
    temp2=year+temp1;
    temp2=temp2%0x7;  //为节省资源,先进行一次取余,避免数大于0xff,避免使用整型数据
    temp2=temp2+day+table_week[month-1];
    if (year%0x4==0&&month<3)temp2-=1;
    week=temp2%0x7;
	return week;
}

//test

void main()
{
    lcd_init();		                   
    c_sun=0;
    year_sun=0x10;
    month_sun=0x10;
    day_sun=0x4;
    Conver_week(c_sun,year_sun,month_sun,day_sun);
    Conversion(c_sun,year_sun,month_sun,day_sun);
	update_disbuf();	
	pro_display();
    while(1);
}
