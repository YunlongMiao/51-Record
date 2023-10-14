/******************************农历转换***************************************/
uchar  moon_shuju[4] = {0,0,0,0};		//用来存储农历数据：月，日
data uchar set[7] = {0x01,0x20,0x14,0x09,0x11,0x07,0x05};//初值：sec,min,hour,day,month,year,week
code uchar year_code[597]=
 				          {
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
				            0x8e,0x93,0x3e,    //2090
				            0x0d,0x52,0x52,    //2091
				            0x0d,0xAA,0x47,    //2092
				            0x66,0xB5,0x3B,    //2093
				            0x05,0x6d,0x4f,    //2094
				            0x04,0xAe,0x45,    //2095
				            0x4A,0x4e,0xB9,    //2096
				            0x0A,0x4d,0x4C,    //2097
				            0x0d,0x15,0x41,    //2098
				            0x2d,0x92,0xB5,    //2099
                    
                           };
code uchar day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};//月份数据表1-9月份（计算农历春节离元旦的天数）
code uint day_code2[3]={0x111,0x130,0x14e};							  //10-11-12月份
bit c_moon;
data uchar year_moon,month_moon,day_moon;
/*子函数,用于读取数据表中农历月的大月或小月,如果该月为大返回1,为小返回0*/
bit get_moon_day(uchar month_p,uint table_addr)
 {
  uchar temp;
  switch (month_p)
   {
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

void Conversion(bit c,uchar year,uchar month,uchar day)//c=0 为21世纪,c=1 为19世纪 输入输出数据均为BCD数据
 {                         
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
  if(c==0)
   {                   
    table_addr=(year+0x64-1)*0x3;
   }
  else 
   {
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
  if(temp1==0x1)
   {  
    temp3=temp2-1;  
   }  
  else
   {
    temp3=temp2+0x1f-1;        
   }
  if (month<10)
   { 
    temp4=day_code1[month-1]+day-1;
   }
  else
   {
    temp4=day_code2[month-10]+day-1;
   }
  if ((month>0x2)&&(year%0x4==0))//如果公历月大于2月并且该年的2月为闰月,天数加1
   {  
    temp4+=1;
   }
  //计算公历日离当年元旦的天数完成
  //判断公历日在春节前还是春节后
  if (temp4>=temp3)//公历日在春节后或就是春节当日使用下面代码进行运算
   { 
    temp4-=temp3;
    month=0x1;
    month_p=0x1;  //month_p为月份指向,公历日在春节前或就是春节当日month_p指向首月
    flag2=get_moon_day(month_p,table_addr); //检查该农历月为大小还是小月,大月返回1,小月返回0
    flag_y=0;
    if(flag2==0)temp1=0x1d; //小月29天
    else temp1=0x1e; //大小30天
    temp2=year_code[table_addr]&0xf0;
    temp2=_cror_(temp2,4);  //从数据表中取该年的闰月月份,如为0则该年无闰月
    while(temp4>=temp1)
	 {
      temp4-=temp1;
      month_p+=1;
      if(month==temp2)
	   {
        flag_y=~flag_y;
        if(flag_y==0)month+=1;
       }
      else 
	  month+=1;
      flag2=get_moon_day(month_p,table_addr);
      if(flag2==0)temp1=0x1d;
      else temp1=0x1e;
     }
    day=temp4+1;
   }
  else  //公历日在春节前使用下面代码进行运算
   {
    temp3-=temp4;
    if (year==0x0){year=0x63;c=1;}
    else year-=1;
    table_addr-=0x3;
    month=0xc;
    temp2=year_code[table_addr]&0xf0;
    temp2=_cror_(temp2,4);
    if (temp2==0)month_p=0xc; 
    else month_p=0xd; //
    flag_y=0;
    flag2=get_moon_day(month_p,table_addr);
    if(flag2==0)temp1=0x1d;
    else temp1=0x1e;
    while(temp3>temp1)
	 {
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
  temp1=year/10;
  temp1=_crol_(temp1,4);
  temp2=year%10;
  Yearl=(year/10)*10+year%10;
  year_moon=temp1|temp2;
  temp1 = month/10;
  moon_shuju[3] = temp1;
  temp2 = month%10;
  moon_shuju[2] = temp2;
  Moon=moon_shuju[3]*10+moon_shuju[2];
  temp1 = day/10;
  moon_shuju[1] = temp1;
  temp2 =day%10;
  moon_shuju[0] = temp2;
  Deay=moon_shuju[1]*10+moon_shuju[0];
 }
/************************农历节日数据库表***********************************/
void days_nl (void)
 {	  
  lcm_w_test(0,0x98);
  /*********************************************************************/
  if (Moon==1&&Deay==1 )   { lcm_w_word ("    春节快乐 !  "); }
  if (Moon==5&&Deay==5 )   { lcm_w_word ("    端午节 !  　"); }
  if (Moon==8&&Deay==15 )  { lcm_w_word ("    中秋节 !    "); }


////////////////////以下由自己加上家人生日//////////////////////////
//  if (Moon==**&&Deay==** ){ lcm_w_word("  **生日！     "); }
//  ..........................................................
//  ..........................................................
//  ...........................................................

/////////////////////////////////////////////////////////////////////



  else//非节日时显示时晨信息
   { 
    if ( hh >= 0x04 && hh <  0x06 ){ lcm_w_word("★★__▲▲__凌晨"); }
	if ( hh >= 0x06 && hh <  0x08 ){ lcm_w_word("☆○__▲△__早晨"); }
	if ( hh >= 0x08 && hh <  0x12 ){ lcm_w_word("__●__▲▲__上午"); }
	if ( hh == 0x12)               { lcm_w_word("____▲●▲__中午"); }
	if ( hh >= 0x13 && hh <  0x18 ){ lcm_w_word("__▲▲__●__下午"); }
	if ( hh >= 0x18 && hh <  0x22 ){ lcm_w_word("△▲__●☆__晚上"); }
	if ( hh >= 0x22 && hh <= 0x23 ){ lcm_w_word("△▲__★☆__夜里"); }
	if ( hh >= 0x00 && hh <  0x04 ){ lcm_w_word("__★▲▲★__深夜"); }
   }
 }
/************************公历节日数据库表***********************************/
void days (void)
 {
  lcm_w_test(0,0x98);
  if ( mo == 0x01 && dd == 0x01 ){ lcm_w_word("Happy New Year!!"); }//1月
  if ( mo == 0x01 && dd == 0x28 ){ lcm_w_word("  世界麻风日    "); }

  if ( mo == 0x02 && dd == 0x02 ){ lcm_w_word("  世界湿地日    "); }//2月
  if ( mo == 0x02 && dd == 0x13 ){ lcm_w_word("  明天情人节了  "); }		
  if ( mo == 0x02 && dd == 0x14 ){ lcm_w_word("  今天是情人节  "); }
		
  if ( mo == 0x03 && dd == 0x01 ){ lcm_w_word("  国际海豹日    "); }//3月		
  if ( mo == 0x03 && dd == 0x03 ){ lcm_w_word("  全国爱耳日    "); }		
  if ( mo == 0x03 && dd == 0x08 ){ lcm_w_word("   3.8妇女节    "); }		
  if ( mo == 0x03 && dd == 0x12 ){ lcm_w_word("    植树节      "); }		
  if ( mo == 0x03 && dd == 0x14 ){ lcm_w_word("  国际警察日    "); }		
  if ( mo == 0x03 && dd == 0x15 ){ lcm_w_word("  消费者权益日  "); }		
  if ( mo == 0x03 && dd == 0x17 ){ lcm_w_word("  国际航海日    "); }		
  if ( mo == 0x03 && dd == 0x21 ){ lcm_w_word("世界森林及睡眠日"); }		
  if ( mo == 0x03 && dd == 0x22 ){ lcm_w_word("    世界水日    "); }		
  if ( mo == 0x03 && dd == 0x23 ){ lcm_w_word("  世界气象日    "); }		
  if ( mo == 0x03 && dd == 0x24 ){ lcm_w_word("世界防治结核病日"); }
		
  if ( mo == 0x04 && dd == 0x01 ){ lcm_w_word("愚人节  小心上当"); }//4		
  if ( mo == 0x04 && dd == 0x07 ){ lcm_w_word("  世界卫生日    "); }		
  if ( mo == 0x04 && dd == 0x08 ){ lcm_w_word("    复活节      "); }		
  if ( mo == 0x04 && dd == 0x13 ){ lcm_w_word("  黑色星期五    "); }
		
  if ( mo == 0x05 && dd == 0x01 ){ lcm_w_word("  劳动节  放假  "); }//5		
  if ( mo == 0x05 && dd == 0x04 ){ lcm_w_word("    青年节      "); }		
  if ( mo == 0x05 && dd == 0x08 ){ lcm_w_word("  世界红十字日  "); }		
  if ( mo == 0x05 && dd == 0x12 ){ lcm_w_word("  国际护士节    "); }		
  if ( mo == 0x05 && dd == 0x05 ){ lcm_w_word("近日注意母亲节  "); }		
  if ( mo == 0x05 && dd == 0x15 ){ lcm_w_word("  国际家庭日    "); }
  if ( mo == 0x05 && dd == 0x31 ){ lcm_w_word("  世界无烟日    "); }		
		
  if ( mo == 0x06 && dd == 0x01 ){ lcm_w_word("  国际儿童节    "); }//6
  if ( mo == 0x06 && dd == 0x05 ){ lcm_w_word("  世界环境日    "); }		
  if ( mo == 0x06 && dd == 0x26 ){ lcm_w_word("  国际禁毒日    "); }		
  if ( mo == 0x06 && dd == 0x06 ){ lcm_w_word("  全国爱眼日    "); }		
  if ( mo == 0x06 && dd == 0x13 ){ lcm_w_word("近日注意父亲节  "); }
  if ( mo == 0x06 && dd == 0x15 ){ lcm_w_word("近日注意父亲节  "); }
		
  if ( mo == 0x07 && dd == 0x01 ){ lcm_w_word("香港回归记念日  "); }//7
  if ( mo == 0x07 && dd == 0x07 ){ lcm_w_word("抗日战争记念日  "); }
  if ( mo == 0x07 && dd == 0x11 ){ lcm_w_word("  世界人口日    "); }

  if ( mo == 0x08 && dd == 0x01 ){ lcm_w_word("  八一建军节    "); }//8
  if ( mo == 0x08 && dd == 0x08 ){ lcm_w_word("  中国男子节    "); }
  if ( mo == 0x08 && dd == 0x15 ){ lcm_w_word("抗战胜利记念日  "); }

  if ( mo == 0x09 && dd == 0x10 ){ lcm_w_word("  中国教师节    "); }//9
  if ( mo == 0x09 && dd == 0x18 ){ lcm_w_word("九·一八事变记念"); }
  if ( mo == 0x09 && dd == 0x20 ){ lcm_w_word("  国际爱牙日    "); }
  if ( mo == 0x09 && dd == 0x27 ){ lcm_w_word("  世界旅游日    "); }

  if ( mo == 0x10 && dd == 0x01 ){ lcm_w_word("  中国国庆节    "); }//10
  if ( mo == 0x10 && dd == 0x04 ){ lcm_w_word("  世界动物日    "); }
  if ( mo == 0x10 && dd == 0x24 ){ lcm_w_word("    联合国日    "); }
  if ( mo == 0x10 && dd == 0x12 ){ lcm_w_word("明天国际教师节  "); }
  if ( mo == 0x10 && dd == 0x13 ){ lcm_w_word("  国际教师节    "); }

  if ( mo == 0x11 && dd == 0x10 ){ lcm_w_word("  世界青年节    "); }//11
  if ( mo == 0x11 && dd == 0x17 ){ lcm_w_word("  世界学生节    "); }

  if ( mo == 0x12 && dd == 0x01 ){ lcm_w_word("  世界艾滋病日  "); }//12
  if ( mo == 0x12 && dd == 0x23 ){ lcm_w_word("  明晚平安夜    "); }
  if ( mo == 0x12 && dd == 0x24 ){ lcm_w_word("  今晚平安夜    "); }
  if ( mo == 0x12 && dd == 0x25 ){ lcm_w_word("    圣诞快乐    "); }
  if ( mo == 0x12 && dd == 0x31 ){ lcm_w_word("    明日新年    "); }

  else
   {//非节日时显示时晨信息 
    if ( hh >= 0x04 && hh <  0x06 ){ lcm_w_word("★★__▲▲__凌晨"); }
	if ( hh >= 0x06 && hh <  0x08 ){ lcm_w_word("☆○__▲△__早晨"); }
	if ( hh >= 0x08 && hh <  0x12 ){ lcm_w_word("__●__▲▲__上午"); }
	if ( hh == 0x12)               { lcm_w_word("____▲●▲__中午"); }
	if ( hh >= 0x13 && hh <  0x18 ){ lcm_w_word("__▲▲__●__下午"); }
	if ( hh >= 0x18 && hh <  0x22 ){ lcm_w_word("△▲__●☆__晚上"); }
	if ( hh >= 0x22 && hh <= 0x23 ){ lcm_w_word("△▲__★☆__夜里"); }
	if ( hh >= 0x00 && hh <  0x04 ){ lcm_w_word("__★▲▲★__深夜"); }
   }
 }
 /***************************向LCM中填写农历数据******************************/
void lcm_w_nl(void)
 {
  unsigned char c_sun,year_sun,month_sun,day_sun;
  lcm_w_test(0,0x81);
  if (Yearl%60==8 ){lcm_w_word("甲子");} //08 
  if (Yearl%60==9 ){lcm_w_word("乙丑");} //09 
  if (Yearl%60==10){lcm_w_word("丙寅");} //08 
  if (Yearl%60==11){lcm_w_word("丁卯");} //09 
  if (Yearl%60==12){lcm_w_word("戊辰");} //08 
  if (Yearl%60==13){lcm_w_word("庚午");} //09 
  if (Yearl%60==14){lcm_w_word("辛未");} //08 
  if (Yearl%60==15){lcm_w_word("壬申");} //09 
  if (Yearl%60==16){lcm_w_word("癸酉");} //08 
  if (Yearl%60==17){lcm_w_word("甲戌");} //09 
  if (Yearl%60==18){lcm_w_word("乙亥");} //08 
  if (Yearl%60==19){lcm_w_word("丙子");} //09 
  lcm_w_test(0,0x83);
  lcm_w_test(1,0x28);
  lcm_w_test(0,0x84);//在屏幕第二行生肖数据库
  if (Yearl%12==8 ){lcm_w_word("鼠");} //08 20
  if (Yearl%12==9 ){lcm_w_word("牛");}  //09 21
  if (Yearl%12==0x0a){lcm_w_word("虎");} //10 22
  if (Yearl%12==0x0b){lcm_w_word("兔");}//11  23
  if (Yearl%12==0){lcm_w_word("龙");}  //12  24
  if (Yearl%12==1){lcm_w_word("蛇");} //13  25
  if (Yearl%12==2){lcm_w_word("马");}  //14 26
  if (Yearl%12==3){lcm_w_word("羊");}  //15 27
  if (Yearl%12==4){lcm_w_word("猴");}  //16 28
  if (Yearl%12==5){lcm_w_word("鸡");}  //17 29
  if (Yearl%12==6){lcm_w_word("狗");}  //18 30
  if (Yearl%12==7){lcm_w_word("猪");}  //19 31
  lcm_w_test(0,0x85);
  lcm_w_test(1,0x29);
  lcm_w_test(0,0x86);
  lcm_w_word("年");
  DS_ReadClock(shuju);
  c_sun = 0;					//2000年后
  year_sun = shuju[5];
  month_sun = shuju[4];
  day_sun = shuju [3];
  Conversion(c_sun,year_sun,month_sun,day_sun);//上电后先转换一次农历数据				
  lcm_w_test(0,0x90);   //显示农历月
  if(Moon==1){  lcm_w_word("正");}
  if(Moon==2){  lcm_w_word("二");}
  if(Moon==3){  lcm_w_word("三");}
  if(Moon==4){  lcm_w_word("四");}
  if(Moon==5){  lcm_w_word("五");}
  if(Moon==6){  lcm_w_word("六");}
  if(Moon==7){  lcm_w_word("七");}
  if(Moon==8){  lcm_w_word("八");}
  if(Moon==9){  lcm_w_word("九");}
  if(Moon==10){  lcm_w_word("十");}
  if(Moon==11){  lcm_w_word("冬");}
  if(Moon==12){  lcm_w_word("腊");}
  lcm_w_test(0,0x91);
  lcm_w_word("月");
  lcm_w_test(0,0x92);   ///显示农历日
  if(Deay<=10)
   {
    if(Deay==1){  lcm_w_word("初一");}
    if(Deay==2){  lcm_w_word("初二");}
    if(Deay==3){  lcm_w_word("初三");}
    if(Deay==4){  lcm_w_word("初四");}
    if(Deay==5){  lcm_w_word("初五");}
    if(Deay==6){  lcm_w_word("初六");}
    if(Deay==7){  lcm_w_word("初七");}
    if(Deay==8){  lcm_w_word("初八");}
    if(Deay==9){  lcm_w_word("初九");}
    if(Deay==10){  lcm_w_word("初十");}
   }
  else
   {
    if(Deay==11){  lcm_w_word("十一");}
    if(Deay==12){  lcm_w_word("十二");}
    if(Deay==13){  lcm_w_word("十三");}
    if(Deay==14){  lcm_w_word("十四");}
    if(Deay==15){  lcm_w_word("十五");}
    if(Deay==16){  lcm_w_word("十六");}
    if(Deay==17){  lcm_w_word("十七");}
    if(Deay==18){  lcm_w_word("十八");}
    if(Deay==19){  lcm_w_word("十九");}
    if(Deay==20){  lcm_w_word("二十");}
    if(Deay==21){  lcm_w_word("廿一");}
    if(Deay==22){  lcm_w_word("廿二");}
    if(Deay==23){  lcm_w_word("廿三");}
    if(Deay==24){  lcm_w_word("廿四");}
    if(Deay==25){  lcm_w_word("廿五");}
    if(Deay==26){  lcm_w_word("廿六");}
    if(Deay==27){  lcm_w_word("廿七");}
    if(Deay==28){  lcm_w_word("廿八");}
    if(Deay==29){  lcm_w_word("廿九");}
    if(Deay==30){  lcm_w_word("三十");}
   }
  lcm_w_nlss();
  lcm_w_xq();
  lcm_w_mm_nl();//刷新 分 
  lcm_w_hh();//刷新 小时 
 }