# include <reg52.h>
# define uchar unsigned char
# define uint unsigned int


//��ʼ��0��1���ⲿ�ж�
void init();

void main()
{
  init();
  while(1);  //�ȴ��жϷ���
}

void init() 
{
  EA = 1;	 		//�������ж�
  EX0 = 1;	 	//����0���ⲿ�ж�
  IT0 = 1;	 	//�����ⲿ�жϴ�����ʽ 
	
  EX1 = 1;   //1���ⲿ�ж�
  IT1 = 1;
}

/****0 - �͵�ƽ����******/
/****1 - �����䴥��******/


void ex0_intr() interrupt 0 
{ 
  P1^0=0;
}

void ex1_intr() interrupt 2 
{ 
  P1^1=0;
}
