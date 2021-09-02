#include<stdio.h>

int main(void)
{
    int a[5];
    int i,s;
    a[0] = a[1] = a[3] = a[4] = 0;  /*a[2] is not init*/
    s = 0;

    for(i=0;i<5;i++){
	s += a[i];
    }

    if (s == 377){
	printf("sum match\n");
    }
    printf("sum: %d\n",s);

    return 0;
}
