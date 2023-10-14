#include "lcd12864.h"
#include "sound.h"

//��8λ����������λд��
void LCD_sendbyte(uchar dat);
//д������
void LCD_write_com(uchar com);
//д������
void LCD_write_dat(uchar dat);
//LCD��ʼ��
void initial_12864();
//LCD��Ļ��ַ��λ
void LCD_Setaddress(uchar x, uchar y);
//LCD��ʾ�ַ����飨���֣�
void LCD_Putstring( uchar x, uchar y, uchar *pData );

void delay(uint n) {
	uint x, y;
	for (x = n; x > 0; x--)
		for (y = 110; y > 0; y--);
}

void Delayms(uint n) {	//��ʱʱ��Ϊ 1ms * x ������11.0592M
	unsigned int i, j;
	for (i = n; i > 0; i--)
		for (j = 110; j > 0; j--);

}


//��8λ����������λд��
void LCD_sendbyte(uchar dat) {
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



//д������
void LCD_write_com(uchar com) {
	LCD_sendbyte(0xf8);          //����5�������ġ�1��������һ������,11111,RW(0),RS(0),0
	LCD_sendbyte(0xf0 &com);    //ȡ����λ�����ݷ����δ��ͣ�ÿ���ֽڵ����ݱ����������ֽ�,����λ���ڵ�һ���ֽڵĸ���λ
	LCD_sendbyte(0xf0 & (com << 4)); //����λ���ڵڶ����ֽڵĸ���λ
	Delayms(1);                   //���в�֧�ֶ����������ɼ��æ��������������ʱ���
}



//д������
void LCD_write_dat(uchar dat) {
	LCD_sendbyte(0xfa);
	LCD_sendbyte(0xf0 & dat);
	LCD_sendbyte(0xf0 & (dat << 4));
	Delayms(1);
}



//LCD��ʼ��
void initial_12864() {
	LCD_write_com(0x30);    //ѡ�����ָ�
	LCD_write_com(0x0c);    //����ʾ�����α꣬������
	LCD_write_com(0x01);    //�����ʾ��Ļ����DDRAMλַ����������Ϊ00H
	Delayms(1);            //��������ʱ��ϳ�1.6ms ��˼Ӵ���ʱ
	LCD_write_com(0x02);    //��DDRAMλַ��λ,�˴�ò���������ظ�
	LCD_write_com(0x06);    //�趨������ƣ�������ʾ���ƶ�
}


//LCD��Ļ��ַ��λ
void LCD_Setaddress(uchar x, uchar y) { //��ַ�ӵ�1�е�1�п�ʼ����0��ʼ��y��
	unsigned char addr;
	switch (x) {
		case 1:
			addr = 0x80 + y - 1;
			break;
		case 2:
			addr = 0x90 + y - 1;
			break;
		case 3:
			addr = 0x88 + y - 1;
			break;
		case 4:
			addr = 0x98 + y - 1;
			break;
		default :
			break;
	}
	LCD_write_com(addr);   //�ַ���ʾ��ʼ��ַ
}




//LCD��ʾ�ַ����飨���֣�
void LCD_Putstring( uchar x, uchar y, uchar *pData ) {
	LCD_Setaddress(x, y);
	while (*pData) {
		LCD_write_dat( *pData++ );
	}
}





void display_distance()
{
	int distance;
	distance = get_Distance();

	if(distance<0)
			LCD_Putstring(3,1,"����");
	else
	{
		LCD_Setaddress(2,1);
		LCD_write_dat(0x30+distance/100 );
		LCD_write_dat(0x30+(distance/10)%10 );
		LCD_write_dat(0x30+distance%10 );
	}
}



void main()
{
	start_sound();
	initial_12864();
	while(1)
	{
//	  LCD_Putstring(3,1,"I Don't Know!!!");
		display_distance();
	}
}
