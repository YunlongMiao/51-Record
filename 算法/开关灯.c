#include<stdio.h>
#define NUM 5000

/*
int text(int s);

int main()
{
    int light[NUM],nam[NUM];
    int N,M;
    int i,j;
    scanf("%d%d",&N,&M);
    for(i=1; i<=N; i++)
        light[i]=0;
    for(i=1; i<=M; i++)
        for(j=i; j<=N; j+=i)
            light[j]=text(light[j]);
        
    for(i=1; i<=N; i++)
    {
        int s=0;
        if(light[i]==1)
            nam[s++]=i;            
    }
        
    for(i=0; i<=sizeof(nam)/4; i++)
    {
        if(i!=sizeof(nam)/4)
            printf("%d,",nam[i]);
        else
            printf("%d",nam[i]);
    }   
}

int text(int s)
{
    if(s==1)
        return 0;

    else
         return 1;   
}*/

int a[5001] ; 
int main(){
    int m, n, k = 1;
    scanf("%d",&n);
    scanf("%d",&m);      
        for(int i = 2; i <= m; i++){
            for(int j = i; j <=n; j=j+i){
                a[j] = a[j]^1;
            }
        }
        printf("%d",k);
        for(int i = 2; i < n+1; i++)
            if(a[i] == 0) printf(",%d",i);
  
//  printf("%d %d %d %d",1^1,2^1,0^1,3^2);
       
}
