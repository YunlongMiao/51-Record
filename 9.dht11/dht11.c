#include <reg51.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int

sbit DH = P1 ^ 1; //定义数据线
uchar rec_dat[9];   //用于显示的接收数据数组


sbit PSB = P2 ^ 4;
sbit SCLK = P2 ^ 5;
sbit SID = P2 ^ 6;



void delay(uint n) {
	uint x, y;
	for (x = n; x > 0; x--)
		for (y = 110; y > 0; y--);
}





void Delayms(uint n) {	//延时时间为 1ms * x 晶振是11.0592M
	unsigned int i, j;
	for (i = n; i > 0; i--)
		for (j = 110; j > 0; j--);

}

//将8位二进制数逐位写入
void LCD_sendbyte(uchar dat) {
	unsigned char i;
	for (i = 0; i < 8; i++) {
		SCLK = 0;
		if (dat & 0x80)
			SID = 1;
		else
			SID = 0;
		SCLK = 1;
		dat = dat << 1;     //左移一位
	}
}



//写入命令
void LCD_write_com(uchar com) {
	LCD_sendbyte(0xf8);          //送入5个连续的“1“，启动一个周期,11111,RW(0),RS(0),0
	LCD_sendbyte(0xf0 &
	             com);    //取高四位，数据分两次传送，每个字节的内容被送入两个字节,高四位放在第一个字节的高四位
	LCD_sendbyte(0xf0 & (com << 4)); //低四位放在第二个字节的高四位
	Delayms(1);                   //串行不支持读操作，不可检测忙操作，这里用延时替代
}



//写入数据
void LCD_write_dat(uchar dat) {
	LCD_sendbyte(0xfa);
	LCD_sendbyte(0xf0 & dat);
	LCD_sendbyte(0xf0 & (dat << 4));
	Delayms(1);
}



//LCD初始化
void LCD_init() {
	LCD_write_com(0x30);    //选择基本指令集
	LCD_write_com(0x0c);    //开显示，无游标，不反白
	LCD_write_com(0x01);    //清除显示屏幕，把DDRAM位址计数器调整为00H
	Delayms(1);            //清屏操作时间较长1.6ms 因此加此延时
	LCD_write_com(0x02);    //清DDRAM位址归位,此处貌似与清屏重复
	LCD_write_com(0x06);    //设定光标右移，整体显示不移动
}


//LCD屏幕地址定位
void LCD_Setaddress(uchar x, uchar y) { //地址从第1行第1列开始不从0开始（y）
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
	LCD_write_com(addr);   //字符显示开始地址
}




//LCD显示字符数组（文字）
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
	uchar R_H, R_L, T_H, T_L, RH, RL, TH, TL, revise;
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
			RL = R_L;
			TH = T_H;
			TL = T_L;
		}
		/*数据处理，方便显示*/
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
		LCD_write_dat(rec_dat[i]);	 //显示数据
}


void Int_232()	//232初始化
{
  TMOD=0x20;
  SCON=0x50;
  TH1=0xFD;
  TL1=0xFD;
  TR1=1;
  TI=0;
}

void TX(uchar TX_char)//串口发送数据
{
  SBUF=TX_char;
  while(!TI);
  TI=0;
}

void TXT()//串口发送给数据
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
	DHT11_delay_ms(1500);    //DHT11上电后要等待1S以越过不稳定状态在此期间不能发送任何指令
   while(1)
   {   
 //      DHT11_delay_ms(1500);    //DHT11上电后要等待1S以越过不稳定状态在此期间不能发送任何指令
       DHT11_receive();
       LCD_write_com(0x80);   //从lcd12864第一行第一个位置开始显示
       for(i=0;i<9;i++)
       LCD_write_dat(rec_dat[i]);   //显示数据    
			 TXT();
   }
}
