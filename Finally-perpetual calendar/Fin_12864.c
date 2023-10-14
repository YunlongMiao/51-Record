#include "Fin_12864.h"
#include"Fin_iic.h"


void Delayms(uint n) 		//��ʱʱ��Ϊ 1ms * x ������11.0592M
{
	unsigned int i, j;
	for (i = n; i > 0; i--)
		for (j = 110; j > 0; j--);
}



void LCD_sendbyte(uchar dat) //��8λ����������λд��
{
	unsigned char i;
	for (i = 0; i < 8; i++) {
		SCLK = 0;
		if (dat & 0x80)
			SID = 1;
		else
			SID = 0;
		SCLK = 1;
		dat = dat << 1;     //����һλ
	}
}




void write_com(uchar com) //д������
{
	LCD_sendbyte(0xf8);          //����5�������ġ�1��������һ������,11111,RW(0),RS(0),0
	LCD_sendbyte(0xf0 & com);    //ȡ����λ�����ݷ����δ��ͣ�ÿ���ֽڵ����ݱ����������ֽ�,����λ���ڵ�һ���ֽڵĸ���λ
	LCD_sendbyte(0xf0 & (com << 4)); //����λ���ڵڶ����ֽڵĸ���λ
	Delayms(1);                   //���в�֧�ֶ����������ɼ��æ��������������ʱ���
}




void write_data(uchar dat) //д������
{
	LCD_sendbyte(0xfa);
	LCD_sendbyte(0xf0 & dat);
	LCD_sendbyte(0xf0 & (dat << 4));
	Delayms(1);
}




void initial_12864()//LCD��ʼ��
{
	write_com(0x30);    //ѡ�����ָ�
	write_com(0x0c);    //����ʾ�����α꣬������
	write_com(0x01);    //�����ʾ��Ļ����DDRAMλַ����������Ϊ00H
	Delayms(1);            //��������ʱ��ϳ�1.6ms ��˼Ӵ���ʱ
	write_com(0x02);    //��DDRAMλַ��λ,�˴�ò���������ظ�
	write_com(0x06);    //�趨������ƣ�������ʾ���ƶ�
}



//void LCD_Setaddress(uchar x, uchar y)  //LCD��Ļ��ַ��λ,��ַ�ӵ�1�е�1�п�ʼ����0��ʼ��y��
//{
//	uchar addr;
//	switch (x) {
//		case 1:
//			addr = 0x80 + y;
//			break;
//		case 2:
//			addr = 0x90 + y;
//			break;
//		case 3:
//			addr = 0x88 + y;
//			break;
//		case 4:
//			addr = 0x98 + y;
//			break;
//		default :
//			break;
//	}
//	write_com(addr);   //�ַ���ʾ��ʼ��ַ
//}



void Putstring(uchar addr,uchar *pData ) //LCD��ʾ�ַ����飨���֣�
{
		write_com(addr);
		while (*pData) 
			write_data( *pData++ );

}
void Show_number(uchar addr,uint num)	  //��ʾ����
{
	char s[6];
	sprintf(s,"%-05d",num);
	Putstring(addr , s);
}
