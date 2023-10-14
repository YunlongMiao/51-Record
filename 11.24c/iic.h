#ifndef __IIC_H__
#define __IIC_H__
//#include"lcd12864.h"
#include "reg52.h"
#include "intrins.h"
#include "string.h"
#define uchar unsigned char
#define uint unsigned int
#define Page_Byte 8 //ҳ�ֽ�8��
#define Max_Addr 0xff//���ɷ��ʵ�ַ
sbit scl=P3^6;
sbit sda=P3^7;

extern uchar temp;
extern void start();//ģ�⿪ʼ�ź�
extern void stop();//ģ��ֹͣ�ź�
extern uchar checkack();//��Ƭ������Ƿ���Ӧ��	
extern void ack();//����Ӧ��
extern void notack();//������Ӧ��
extern void iic_write_byte(uchar dat );//IICдһ���ֽ�
extern uchar iic_read_byte();//IIc��һ���ֽ�
extern void display_iic_byte(uchar temp);

extern void iic_write_only(uchar x,uchar y);//���ֽ�д��
extern uchar iic_read_only(uchar x);//���ֽڶ�ȡ
extern void iic_write_more(uchar x,uchar *buffer,uchar num);//ҳд��
extern void iic_read_more(uchar x,uchar *buffer,uchar num);//ҳ��
extern uchar AT24C02_Write(uchar WriteAddr,uchar *buffer) ;////��ҳд������


#endif