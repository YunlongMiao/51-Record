#include<reg52.h>
#include<intrins.h>        
#define uchar unsigned char
#define uint unsigned int
#define LCD1602_DB  P0      
sbit LCD1602_RS = P2^0;
sbit LCD1602_RW = P2^1;
sbit LCD1602_EN = P2^2;         
sbit Trig = P1^0;
sbit Echo = P1^1;
uchar PuZh[]="The Distance: ";
uchar code ASCII[15] =    {'0','1','2','3','4','5','6','7','8','9','.','-','C','M'};
void Read_Busy()   
{
         uchar sta;     
    LCD1602_DB = 0xff;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do
    {
        LCD1602_EN = 1;
        sta = LCD1602_DB;
        LCD1602_EN = 0;   
    }while(sta & 0x80);
}
void delayms(uint ms)
{
        uchar i=100,j;
        for(;ms;ms--)
        {
                while(--i)
                {
                        j=10;
                        while(--j);
                }
        }
}
void Lcd1602_Write_Cmd(uchar cmd)     
{
    Read_Busy();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_EN = 1;
    LCD1602_EN = 0;   
}
void Lcd1602_Write_Data(uchar dat)  
        {
      Read_Busy();
      LCD1602_RS = 1;
      LCD1602_RW = 0;
      LCD1602_DB = dat;
      LCD1602_EN = 1;
      LCD1602_EN = 0;
}
        void LcdSetCursor(uchar x,uchar y)
                {
    uchar addr;
    if(y == 0)
        addr = 0x80 + x;
    else
        addr = 0x40 + x;

    Lcd1602_Write_Cmd(addr|0x80);
}
        void DisplayOneChar(uchar X, unsigned char Y, uchar DData)
{
        Y &= 0x1;
        X &= 0xF;
        if (Y) X |= 0x40;
        X |= 0x80;
        Lcd1602_Write_Cmd(X);
        Lcd1602_Write_Data(DData);
}
void LcdShowStr(uchar x,uchar y,uchar *str)
{
    LcdSetCursor(x,y);     
    while(*str != '\0')
    {
        Lcd1602_Write_Data(*str++);
    }
}
void InitLcd1602()  
{
    Lcd1602_Write_Cmd(0x38);   
    Lcd1602_Write_Cmd(0x0c);
    Lcd1602_Write_Cmd(0x06);
    Lcd1602_Write_Cmd(0x01);   
}
static uchar DisNum = 0;                  
       uint  time=0;
           unsigned long S=0;
           bit      flag =0;
           uchar disbuff[4]           ={ 0,0,0,0,};
void Conut(void)
        {
         time=TH0*256+TL0;
         TH0=0;
         TL0=0;
        
         S=(time*1.7)/100;     
//         if((S>=700)||flag==1)
//         {         
//          flag=0;
         
          DisplayOneChar(0, 1, ASCII[11]);
          DisplayOneChar(1, 1, ASCII[10]);        
          DisplayOneChar(2, 1, ASCII[11]);
          DisplayOneChar(3, 1, ASCII[11]);
          DisplayOneChar(4, 1, ASCII[12]);        
//         }
//         else
//         {
          disbuff[0]=S/100;
          disbuff[1]=S%100/10;
          disbuff[2]=S%100%10;
          DisplayOneChar(0, 1, ASCII[disbuff[0]]);
          DisplayOneChar(1, 1, ASCII[10]);        
          DisplayOneChar(2, 1, ASCII[disbuff[1]]);
          DisplayOneChar(3, 1, ASCII[disbuff[2]]);
          DisplayOneChar(4, 1, ASCII[12]);        
                DisplayOneChar(5, 1, ASCII[13]);        
 //        }
        }
void zd0() interrupt 1
{
    flag=1;                                                         
}
void  StartModule()                          
  {
          Trig=1;                                             
          _nop_();  _nop_(); _nop_(); _nop_();
          _nop_();  _nop_(); _nop_(); _nop_();
          _nop_();  _nop_(); _nop_(); _nop_();
          _nop_();  _nop_(); _nop_(); _nop_();
                _nop_();  _nop_(); _nop_(); _nop_();
          Trig=0;
  }
        void main(void)
{

         TMOD=0x01;                  
         TH0=0;
         TL0=0;         
         ET0=1;            
         EA=1;                           
				InitLcd1602();
        LcdShowStr(0,0,PuZh);
        while(1)
        {
             StartModule();
             while(!Echo);               
             TR0=1;                           
             while(Echo);                        
             TR0=0;                                
						Conut();                        
                 delayms(80);
        }
                        
}