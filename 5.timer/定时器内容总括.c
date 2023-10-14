#include <reg52.h>
unsigned char PWM_COUNT; //计数
unsigned int HUXI_COUNT;    //占空比更新时间
unsigned char PWM_VLAUE;    //占空比比对值
bit direc_flag;             //占空比更新方向

void timer0_init()
{
    TMOD=0x02;          //模式设置，00010000，定时器0，工作于模式2（M1=1，M0=0）
    TH0=0x47;               //定时器0溢出值设置，每隔200us发起一次中断。
    TL0=0X47;
    TR0=1;                  //定时器0开始计时	启动定时器/计数器0启动位，功能和TR1类似~~TCON寄存器
    ET0=1;                  //开定时器0中断     Timer0的溢出中断允许位~~~IE寄存器
    EA=1;                       //开总中断
    PWM_COUNT =0;
}

				/*TMOD   设置定时器工作方式（4种）

						方式0	0x00:	13位定时器/计数器
						方式1	0x01:	16位定时器/计数器
						方式2	0x10:	自动重载8位定时器/计数器）
						方式3	0x11:	T0分为2个8位独立计数器，T1无方式3

								TMOD=0x10 ：

								设置定时器1为方式1模式（16位定时器/计数器）。

								TMOD=0x02：

								设置定时器2为方式2模式（自动重载8位定时器/计数器）
				*/

方式3：
当选择方式3时，定时器T0就会被分成两个独立的计数器或者定时器。
此时，TL0为8位计数器，计数溢出好置位TF0，并向CPU申请中断，之后需要软件重装初值； 
TH0也被固定为8位计数器，不过TL0已经占用了TF0和TR0，因此TH0将占T1的中断请求标志TF1和定时器启动控制为TR1



							/*  中断寄存器内容（折叠）
							IE – 中断允许控制寄存器
							IP – 中断优先级控制寄存器
							TMOD – s
							TCON – 定时器控制寄存器
							SCON – 串口控制寄存器
							THx/TLx – 定时器初值寄存器
							*/
void time0() interrupt 1
{   
    PWM_COUNT++;
    HUXI_COUNT++;
    if(PWM_COUNT == PWM_VLAUE)      //判断是否到了点亮LED的时候
        LED = 1;                    //点亮LED
    if(PWM_COUNT == 10)             //当前周期结束
    {
        LED = 0;                    //熄灭LED
        PWM_COUNT = 0;              //重新计时
    }
    if((HUXI_COUNT == 600) && (direc_flag == 0))
    {                               //占空比增加10%
        HUXI_COUNT = 0;
        PWM_VLAUE++;
        if(PWM_VLAUE == 9)          //占空比更改方向
            direc_flag = 1; 
    }
    if((HUXI_COUNT == 600) && (direc_flag == 1))
    {                               //占空比减少10%
        HUXI_COUNT = 0;
        PWM_VLAUE--;
        if(PWM_VLAUE == 1)          //占空比更改方向
            direc_flag = 0; 
    }   
}
void main()
{
    HUXI_COUNT = 0;
    PWM_COUNT = 0;
    PWM_VLAUE = 5;
    direc_flag = 0;
    LED = 1;            //默认LED熄灭   
    timer0_init();      //定时器0初始化
    while(1);
}