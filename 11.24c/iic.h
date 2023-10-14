#ifndef __IIC_H__
#define __IIC_H__
//#include"lcd12864.h"
#include "reg52.h"
#include "intrins.h"
#include "string.h"
#define uchar unsigned char
#define uint unsigned int
#define Page_Byte 8 //页字节8个
#define Max_Addr 0xff//最大可访问地址
sbit scl=P3^6;
sbit sda=P3^7;

extern uchar temp;
extern void start();//模拟开始信号
extern void stop();//模拟停止信号
extern uchar checkack();//单片机检测是否有应答	
extern void ack();//产生应答
extern void notack();//产生非应答
extern void iic_write_byte(uchar dat );//IIC写一个字节
extern uchar iic_read_byte();//IIc读一个字节
extern void display_iic_byte(uchar temp);

extern void iic_write_only(uchar x,uchar y);//单字节写入
extern uchar iic_read_only(uchar x);//单字节读取
extern void iic_write_more(uchar x,uchar *buffer,uchar num);//页写入
extern void iic_read_more(uchar x,uchar *buffer,uchar num);//页读
extern uchar AT24C02_Write(uchar WriteAddr,uchar *buffer) ;////跨页写入数组


#endif