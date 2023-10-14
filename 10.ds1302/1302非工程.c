#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int

sbit PSB = P2 ^ 4;//CS-RS
sbit SCLK = P2 ^ 5;//E
sbit SID = P2 ^ 6;//RW

sbit clk=P0^2;
sbit io=P0^3;
sbit ce=P0^4;

uchar add_write[]={0x8c,0x8a,0x88,0x86,0x84,0x82,0x80};
uchar add_read[]={0x8d,0x8b,0x89,0x87,0x85,0x83,0x81};
uchar tim0[]={20,1,4,13,9,10,26};//�ꡢ���ڡ��¡��ա�ʱ������

uchar tim_bcd[7];//���������ʱ������
uchar tim[14];//����ֽ⿪��ʮλ����λ

//��8λ����������λд��
void LCD_sendbyte(uchar dat);
//д������
void LCD_write_com(uchar com);
//д������
void LCD_write_dat(uchar dat);
//LCD��ʼ��
void LCD_init();
//LCD��Ļ��ַ��λ
void LCD_Setaddress(uchar x, uchar y);
////LCD��ʾ�ַ����飨���֣�
//void LCD_Putstring( uchar x, uchar y, uchar *pData );

void display_1302();
void write_1302_8(uchar dat);
void write_1302_byte(uchar add,uchar dat);
uchar read_1302_byte(uchar add);
void ds1302_initial();
void read_1302time();


void Delayms(uint n) 		//��ʱʱ��Ϊ 1ms * x ������11.0592M
{
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
void write_com(uchar com) {
	LCD_sendbyte(0xf8);          //����5�������ġ�1��������һ������,11111,RW(0),RS(0),0
	LCD_sendbyte(0xf0 & com);    //ȡ����λ�����ݷ����δ��ͣ�ÿ���ֽڵ����ݱ����������ֽ�,����λ���ڵ�һ���ֽڵĸ���λ
	LCD_sendbyte(0xf0 & (com << 4)); //����λ���ڵڶ����ֽڵĸ���λ
	Delayms(1);                   //���в�֧�ֶ����������ɼ��æ��������������ʱ���
}



//д������
void write_data(uchar dat) {
	LCD_sendbyte(0xfa);
	LCD_sendbyte(0xf0 & dat);
	LCD_sendbyte(0xf0 & (dat << 4));
	Delayms(1);
}



//LCD��ʼ��
void initial_12864() {
	write_com(0x30);    //ѡ�����ָ�
	write_com(0x0c);    //����ʾ�����α꣬������
	write_com(0x01);    //�����ʾ��Ļ����DDRAMλַ����������Ϊ00H
	Delayms(1);            //��������ʱ��ϳ�1.6ms ��˼Ӵ���ʱ
	write_com(0x02);    //��DDRAMλַ��λ,�˴�ò���������ظ�
	write_com(0x06);    //�趨������ƣ�������ʾ���ƶ�
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
	write_com(addr);   //�ַ���ʾ��ʼ��ַ
}




////LCD��ʾ�ַ����飨���֣�
//void LCD_Putstring( uchar x, uchar y, uchar *pData ) {
//	LCD_Setaddress(x, y);
//	while (*pData) {
//		write_data( *pData++ );
//	}
//}

/****************дһ���ֽ�*************/
/*************д��ַ��д���ݣ���ʽ��**************/

void write_1302_8(uchar dat)
{
	uchar i,temp ;
	for(i=0; i<8; i++)
	{
		clk = 0;//ʱ���ź�Ϊ��ʱ�����ݲ��ܸı䣬ͬʱΪ������������׼��
		temp  = dat&0x01;//ȡ�����λ
		if(temp )
			io =1;//�����һλΪ1��io����
		else 
			io=0;//�����һλΪ0��io����
		clk = 1;//���������أ�1302������
		dat=dat>>1;//Ϊ�ͳ���һλ��׼��
		
	}
}

/*****************д����******************/
/*************д��ַ��д���ݣ�Ŀ�ģ�**************/

void write_1302_byte(uchar add,uchar dat)
{
	ce =0;//�����в���ʱ����
	clk = 0;//ʱ���߿�ʼǰΪ��
	ce =1;//��������һ��д��������
	write_1302_8(add );//�͵�ַ�ֽ�
	write_1302_8(dat );//�������ֽ�
	ce =0;//һ��д�����Ľ���
}


/*****************������*************/
/*************д��ַ�������ݣ�sclk��**************/

uchar read_1302_byte(uchar add)
{
	uchar i,value;
	ce =0;
	clk =0;
	ce =1;//��ʼһ�ζ�����
	write_1302_8(add );//�͵�ַ�ֽ�
	for(i=0; i<8; i++)
	{
		clk =0;//������ʱ�½�����Ч
		value =value >>1;//����һ�α���������Ƶ��θ�λ
		if(io)//�����Ϊ1
			value =value|0x80;//��1�ŵ����λ
		clk =1;//������һλ��ʱ�������ߣ�Ϊ�ٴγ����½�����׼��
	
	}
	ce =0;//����һ�ζ���������
	return value ;//����ȡ��1�ֽ����ݴ��ݳ�ȥ
}



/***************��ʼ��**************/
/**********ʱ�䡢���ڵĳ�ʼֵ����**********/

void ds1302_initial()
{
	uchar i,temp;
	for(i=0; i<7; i++)//���Ĵ�������ֵ֮ǰ�������ڡ�ʱ��ת��ΪBCD��
	{
		temp = tim0[i]/10;//ȡʮλ
		tim0[i]=temp*16+tim0[i]%10;//bcd ��
	}
	
	write_1302_byte(0x8e,0x00);//���Ĵ������ó�ʼֵ֮ǰ����Ҫȥ��д����
	
	for(i=0; i<7; i++)//ÿ��ѭ����һ���Ĵ�������ֵ
	{
		write_1302_byte(add_write[i],tim0[i]);
	}
	
	write_1302_byte(0x8e,0x80);//���Ĵ������ó�ʼֵ����Ҫ����д����
	

}



/***************��ȡʱ�䲢����ת��*************/

void read_1302time()
{
	uchar i;
	for(i=0; i<7; i++)//ÿ��ѭ����һ���Ĵ������ݣ����浽tim_bcd[]������
	{
		tim_bcd[i]=read_1302_byte(add_read[i]);
	}
	
	for(i=0; i<7; i++)//�����������ڡ�ʱ��ֽ�Ϊʮλ�͸�λ
	{
		tim[2*i]=tim_bcd[i]/16;
		tim[2*i+1]=tim_bcd[i]%16;
	}
}





//void Int_232()	//2323?��??��
//{
//  TMOD=0x20;
//  SCON=0x50;
//  TH1=0xFD;
//  TL1=0xFD;
//  TR1=1;
//  TI=0;
//}

//void TX(uchar TX_char)//��??������?����y?Y
//{
//  SBUF=TX_char;
//  while(!TI);
//  TI=0;
//}

//void TXT()//��??������?��??��y?Y
//{ 
//			uchar i; 
//			for(i=0;i<13;i++)
//			 {
//				 TX(tim[i]);	
//					
//			 }
//}




/***************��ʾ���ں�ʱ��**************/

void display_1302()
{
	//Int_232();

	LCD_Setaddress(1,1);
	write_data(0x30+tim[0]);//��
	write_data(0x30+tim[1]);
	write_data('-');
	write_data(0x30+tim[4]);//��
	write_data(0x30+tim[5]);
	write_data('-');
	write_data(0x30+tim[6]);//��
	write_data(0x30+tim[7]);
	write_data(' ');
	write_data(0x30+tim[2]);//��
	write_data(0x30+tim[3]);
	

	LCD_Setaddress(2,1);
	write_data(0x30+tim[8]);
	write_data(0x30+tim[9]);
	write_data(':');
	write_data(0x30+tim[10]);
	write_data(0x30+tim[11]);
	write_data(':');
	write_data(0x30+tim[12]);
	write_data(0x30+tim[13]);
//	TXT();
}



void main()
{
	initial_12864();
	ds1302_initial();
	while(1)
	{	
		read_1302time();
		display_1302();

	}

}