#include <stdio.h>
#include <time.h>

void multiply(int F[2][2], int M[2][2]);

void power(int F[2][2], int n);

/* function that returns nth Fibonacci number */
int fib3(int n)
{
  int F[2][2] = {{1,1},{1,0}};
  if (n == 0)
    return 0;
  power(F, n-1);
  return F[0][0];
}

/* Optimized version of power() in method 4 */
void power(int F[2][2], int n)
{
  if( n == 0 || n == 1)
      return;
  int M[2][2] = {{1,1},{1,0}};

  power(F, n/2);
  multiply(F, F);

  if (n%2 != 0)
     multiply(F, M);
}

void multiply(int F[2][2], int M[2][2])
{
  int x =  F[0][0]*M[0][0] + F[0][1]*M[1][0];
  int y =  F[0][0]*M[0][1] + F[0][1]*M[1][1];
  int z =  F[1][0]*M[0][0] + F[1][1]*M[1][0];
  int w =  F[1][0]*M[0][1] + F[1][1]*M[1][1];

  F[0][0] = x;
  F[0][1] = y;
  F[1][0] = z;
  F[1][1] = w;
}


int fib2(int n)
{
    int ret;
    if(n == 0){
        return 0;
    } else if(n <= 2){
        return 1;
    }

    ret = fib2(n-1)+fib2(n-2);
    /*printf("%d ",ret);*/
    return ret;
}

int fib1(int n)
{
    int i;
    int map[n+1];
    map[0]=0;
    map[1]=1;
    /*printf("0 1 ");*/

    for(i=2;i<=n;i++){
        map[i]=map[i-1]+map[i-2];
	/*printf("%d ",map[i]);*/
    }
    /*printf("\n");*/
    return map[i-1];
}

int fib(int n)
{
    int i;
    int map[3];
    map[0] = 0;
    map[1] = 1;
    map[2] = 1;
    /*printf("0 1 1 ");*/

    for(i=2;i<n;i++){
        map[0]=map[1];
        map[1]=map[2];
        map[2]=map[2-1]+map[2-2];
        /*printf("%d ",map[2]);*/
    }
    /*printf("\n");*/

    return map[2];
}

int timeCal( int(*fn)(int n),int n,char *fnName)
{
    struct timespec tp1,tp2;
    int ret;
    clockid_t clkid = CLOCK_REALTIME;
    printf("func:%s\n	",fnName);
    clock_gettime(clkid,&tp1);
    ret = fn(n);
    clock_gettime(clkid,&tp2);
    printf("	used %lds %ldns,is:%d\n",tp2.tv_sec - tp1.tv_sec,
					tp2.tv_nsec - tp1.tv_nsec,
					ret);

    return 0;
}


#define FN_STR(fn) #fn
int main(int argc,char** argv)
{
    int n;

    printf("input fib level n:");
    scanf("%d",&n);
    printf("fib level:%d\n",n);

    timeCal(fib,n,FN_STR(fib));
    printf("\n");
    timeCal(fib1,n,FN_STR(fib1));
    printf("\n");
    /*
     *timeCal(fib2,n,FN_STR(fib2));
     *printf("\n");
     */
    timeCal(fib3,n,FN_STR(fib3));
    printf("\n");
    return 0;
}
