
sbit scl
sbit sda 


void delay_us()
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();

}


/*********************************************************/
void start()//模拟开始信号
{
	scl=1;//时钟线为高
	sda=1;//数据线为高，为下降沿做准备
	delay_us();//满足时序要求(>4.7us)
	sda=0;//数据线从高到低，出现下降沿
	delay_us();//满足时序要求(>4us)
}
/****************************************************************/
void stop()//模拟停止信号
{
	sda=0;//数据线为低，为上升沿做准备
	scl=1;//时钟线为高
	delay_us();//满足时序要求(>4us )
	sda=1;//数据线从低到高，出现上升沿
	delay_us();//满足时序要求(>4us )
}

void checkack()//单片机检测是否有应答
{
	uchar i;
	scl=0;//第9个时钟信号的低电平期间
	sda=1;//主机释放sda，使从机能拉低sda
	delay_us();//让从机有足够时间拉低sda
	scl=1;//第9个时钟信号的低电平期间
	delay_us();//让从机有足够时间拉低sda
	while((sda==1)&&(i<250))
		i++;//两条件同时成立执行+1
	while(sda==1)
	{
		i++;
		if(i>250)
		{
			stop();
			return 1;
		}
	}
	
	
}


void ack()//产生应答
{
	scl=0;//开始第9个时钟信号的低电平
	sda=0;//主机拉低sda以产生应答信号“0”
	delay_us();//延时使低电平未定
	scl=1;//开始第9个时钟信号的高电平
	delay_us();//使从机有足够时间检测“0”
	scl=0;//第9个时钟周期结束
}


void noack()//不产生应答
{
	scl=0;//开始第9个时钟信号的低电平
	sda=1;//置高sda以表示不作应答
	delay_us();//保证信号“1”稳定
	scl=1;//开始第9个时钟信号的高电平
	delay_us();//使从机有足够时间检测“1”
	scl=0;//第9个时钟周期结束
}

void iic_write_byte(uchat dat)//IIC写一个字节
{
	uchar i;
	scl=0;//时钟线拉低，协议规定只有在时钟线为低时才能改变数据状态
	for(i=0; i<8; i++)
	{
		sda=dat&0x80;//iIC写入从高位开始
		scl=1;//时钟线拉高，从机开始读取当前位
		delay_us();//为读取提供时间
		dat=dat<<1;//将下一位移到最高位
		scl=0;//时钟线拉低，为下一次写入做准备
	}
	sda=1;//释放总线，方便从机应答
}

uchar iic_read_byte()//IIc读一个字节
{
	uchar i,value;
	sda=1;//确保数据线为释放状态，使从机能往sda上放数据
	scl=0;//确保时钟线为低
	for(i=0; i<8; i++)
	{
		value=value<<1;//每次读取数据放在最低位
		scl=1;//时钟线拉高，从机不能改变sda数据
		delay_us();//使sda上电平稳定
		if(sda)
			value=value|0x01;//将送来的1放在最低位
		scl=0;//时钟线拉低，允许从机送下一位数据
		delay_us();//为从机送数据提供时间
	}
	return value;//返回数据
}