#include <reg51.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int

sbit DH = P1 ^ 1; //����������
uchar rec_dat[9];   //������ʾ�Ľ�����������


sbit PSB = P2 ^ 4;
sbit SCLK = P2 ^ 5;
sbit SID = P2 ^ 6;



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
	LCD_sendbyte(0xf0 &
	             com);    //ȡ����λ�����ݷ����δ��ͣ�ÿ���ֽڵ����ݱ����������ֽ�,����λ���ڵ�һ���ֽڵĸ���λ
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
void LCD_init() {
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










void DHT11_delay_us(uchar n) {
	while (--n);
}

void DHT11_delay_ms(uint z) {
	uint i, j;
	for (i = z; i > 0; i--)
		for (j = 110; j > 0; j--);
}

void DHT11_start() {
	DH = 1;
	DHT11_delay_us(2);
	DH = 0;
	DHT11_delay_ms(30);   //��ʱ18ms����
	DH = 1;
	DHT11_delay_us(30);
}

uchar DHT11_rec_byte() {    //����һ���ֽ�
	uchar i, dat = 0;
	for (i = 0; i < 8; i++) { //�Ӹߵ������ν���8λ����
		while (!DH);  ////�ȴ�50us�͵�ƽ��ȥ
		DHT11_delay_us(8);     //��ʱ60us�������Ϊ��������Ϊ1������Ϊ0
		dat <<= 1;         //��λʹ��ȷ����8λ���ݣ�����Ϊ0ʱֱ����λ
		if (DH == 1) //����Ϊ1ʱ��ʹdat��1����������1
			dat += 1;
		while (DH); //�ȴ�����������
	}
	return dat;
}

void DHT11_receive() {    //����40λ������
	uchar R_H, R_L, T_H, T_L, RH, RL, TH, TL, revise;
	DHT11_start();
	if (DH == 0) {
		while (DH == 0); //�ȴ�����
		DHT11_delay_us(40);  //���ߺ���ʱ80us
		R_H = DHT11_rec_byte();  //����ʪ�ȸ߰�λ
		R_L = DHT11_rec_byte();  //����ʪ�ȵͰ�λ
		T_H = DHT11_rec_byte();  //�����¶ȸ߰�λ
		T_L = DHT11_rec_byte();  //�����¶ȵͰ�λ
		revise = DHT11_rec_byte(); //����У��λ

		DHT11_delay_us(25);    //����

		if ((R_H + R_L + T_H + T_L) == revise) { //У��
			RH = R_H;
			RL = R_L;
			TH = T_H;
			TL = T_L;
		}
		/*���ݴ���������ʾ*/
				rec_dat[0]=0x30+(RH/10);
        rec_dat[1]=0x30+(RH%10);				
        rec_dat[2]=0x52;//"R"
        rec_dat[3]=0x48;//"H"
        rec_dat[4]=0x20;
        rec_dat[5]=0x30+(TH/10);
        rec_dat[6]=0x30+(TH%10);
        rec_dat[7]=0x43;//"C"
	}
}

void display()
{
	uchar i;
	LCD_Setaddress(1,0);
	for (i = 0; i < 8; i++)
		LCD_write_dat(rec_dat[i]);	 //��ʾ����
}


void Int_232()	//232��ʼ��
{
  TMOD=0x20;
  SCON=0x50;
  TH1=0xFD;
  TL1=0xFD;
  TR1=1;
  TI=0;
}

void TX(uchar TX_char)//���ڷ�������
{
  SBUF=TX_char;
  while(!TI);
  TI=0;
}

void TXT()//���ڷ��͸�����
{ 
			uchar i; 
			for(i=0;i<8;i++)
			 {
				 TX(rec_dat[i]);	
					Delayms(50);
			 }
}



void main()
{
   uchar i;   
   LCD_init();
	 Int_232();
	DHT11_delay_ms(1500);    //DHT11�ϵ��Ҫ�ȴ�1S��Խ�����ȶ�״̬�ڴ��ڼ䲻�ܷ����κ�ָ��
   while(1)
   {   
 //      DHT11_delay_ms(1500);    //DHT11�ϵ��Ҫ�ȴ�1S��Խ�����ȶ�״̬�ڴ��ڼ䲻�ܷ����κ�ָ��
       DHT11_receive();
       LCD_write_com(0x80);   //��lcd12864��һ�е�һ��λ�ÿ�ʼ��ʾ
       for(i=0;i<9;i++)
       LCD_write_dat(rec_dat[i]);   //��ʾ����    
			 TXT();
   }
}
