#include"Fin_dht11.h"
#include "Fin_12864.h"
uchar rec_dat[2];   //������ʾ�Ľ�����������


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
	uchar R_H, R_L, T_H, T_L, RH, TH, revise;//RL, TL,
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
			TH = T_H;
		}
		/*���ݴ���������ʾ*/
				rec_dat[0]=RH;
        rec_dat[1]=TH;				
	}
}

void display_DHT11()
{
		DHT11_receive();
		Show_number(0x98,rec_dat[1]);
		write_com(0x99);
		write_data(0x43);
		write_data(0x20);
		Show_number(0x9a,rec_dat[0]);
		Putstring(0x9b,"RH");
}




