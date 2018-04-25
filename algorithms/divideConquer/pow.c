#include <stdio.h>

/*recurive method*/
/*递归不是目的,目的是节约时间*/
int mypow(int x,int n)
{
    if(n ==0){
	return 1;
    }

    if(n == 1){
        return x;
    }
    return x*mypow(x,n-1);
}

int mypow2(int x,int n)
{
    long ret=1;
    while(n){
	ret = ret*x;
	n--;
    }

    return ret;
}

/* Function to calculate x raised to the power y */
/*divide-and-conquer*/
int power(int x, unsigned int y)
{
    if (y == 0){
        return 1;
    }else if (y%2 == 0){
        return power(x, y/2)*power(x, y/2);
    }else{
        return x*power(x, y/2)*power(x, y/2);
	/*如果为奇数/2,则余数1,需要*x弥补*/
    }
}

int main() {
    int t,n;
    long ret,ret2,ret3;
    scanf("%d",&t);
    scanf("%d",&n);

    printf("%d^%d\n",t,n);

    ret = mypow(t,n);
    ret2 = mypow2(t,n);
    ret3 = power(t,n);
    printf("ret:%ld,%#lx\n",ret,ret);
    printf("ret2:%ld,%#lx\n",ret2,ret2);
    printf("ret3:%ld,%#lx\n",ret3,ret3);
    return 0;
}
