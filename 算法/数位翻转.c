#include<stdio.h>
int main()
{
	int n;
	int x,a,b,c,d;
	scanf("%d",&n);
	a=n;
	b=n-1;
	x=0;
	while(a!=0)
	{
		c=a&0x01;
		d=b&0x01;
		if(c!=d)
			x++;
		a=a>>1;
		b=b>>1;
	}
	
	printf("%d",x);
 } 
 
/* 	1 1 	+			0
	2 10 -		00 01 
	3 11  	+	10
	4 100 -
	5 101  +
	6 110 - 
	7 111	+
	8 1000 - 
	9 1001+
	10 1010- 
	11  1011	+
*/
