#include<stdio.h>
#include <ctype.h>
char text(char x);
void main()
{
    char cha[50];
    char ahc[50];//1 2 3 24 25 26
//    char ss[29]={a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,a,b,c};
    int i,temp;
    for(i=0; i<50; i++)
    {
        scanf("%s",&cha[i]);
        if(!isalpha(cha[i]))
        {
            temp=i;
            break;
        }             
    }
    for(i=0; i<temp; i++)
    {
        int x;
        x=text(cha[temp-i-1]);
        ahc[i]=x;
    }
    for(i=0; i<temp; i++)
    {
        if ((ahc[i] > 'C' && ahc[i] < 'Z')||(ahc[i] > 'c' && ahc[i] < 'z'))
            ahc[i]=ahc[i]-3;
        else
            ahc[i]=ahc[i]+23;
    }
    
    for(i=0; i<temp; i++)
        printf("%s",ahc[i]);
    
}

char text(char x)
{
    if (x > 'A' && x < 'Z')
	{
		x=x + 32;
        return x;
	}
	else if(x>'a'&&x<'z')
	{
		x=x - 32;
        return x;
	}
}
