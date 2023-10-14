#include "lcd12864.h"
#include "sound.h"
void display_distance()
{
	int distance;
	distance = get_Distance();

	if(distance<0)
			LCD_Putstring(3,1,"´íÎó");
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
	  LCD_Putstring(3,1,"I Don't Know!!!");
		display_distance();
	}
}




