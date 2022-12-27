#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/*
 *Ref
 *    https://blog.csdn.net/qq_39445165/article/details/94891900
 *    http://c.biancheng.net/view/2043.html
 */

int main(int argc,char **argv)
{
    int i = 0;
    int a = 10000;
    int b = 60000;

    for (i=0; i<10; i++){
        printf("rand:%d\n",rand()%10);                //[0,10) 之间的随机数
    }
    printf("-----------------\n");

    for (i=0; i<10; i++){
        printf("rand:%d\n",rand()%51+13);             //[13-(51+13)), [13~64)间的随机数
    }
    printf("-----------------\n");

    for (i=0; i<10; i++){
        printf("rand:%d\n", (int) ((1.0*rand()/RAND_MAX)*(b-a) + a));         //[a,b) 之间的随机数, 生成大于RAND_MAX的数.
    }
}
