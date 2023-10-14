#include"Fin_dht11.h"
#include "Fin_12864.h"
uchar rec_dat[2];   //用于显示的接收数据数组


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
	DHT11_delay_ms(30);   //延时18ms以上
	DH = 1;
	DHT11_delay_us(30);
}

uchar DHT11_rec_byte() {    //接收一个字节
	uchar i, dat = 0;
	for (i = 0; i < 8; i++) { //从高到低依次接收8位数据
		while (!DH);  ////等待50us低电平过去
		DHT11_delay_us(8);     //延时60us，如果还为高则数据为1，否则为0
		dat <<= 1;         //移位使正确接收8位数据，数据为0时直接移位
		if (DH == 1) //数据为1时，使dat加1来接收数据1
			dat += 1;
		while (DH); //等待数据线拉低
	}
	return dat;
}

void DHT11_receive() {    //接收40位的数据
	uchar R_H, R_L, T_H, T_L, RH, TH, revise;//RL, TL,
	DHT11_start();
	if (DH == 0) {
		while (DH == 0); //等待拉高
		DHT11_delay_us(40);  //拉高后延时80us
		R_H = DHT11_rec_byte();  //接收湿度高八位
		R_L = DHT11_rec_byte();  //接收湿度低八位
		T_H = DHT11_rec_byte();  //接收温度高八位
		T_L = DHT11_rec_byte();  //接收温度低八位
		revise = DHT11_rec_byte(); //接收校正位

		DHT11_delay_us(25);    //结束

		if ((R_H + R_L + T_H + T_L) == revise) { //校正
			RH = R_H;
			TH = T_H;
		}
		/*数据处理，方便显示*/
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




