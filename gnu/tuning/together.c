#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define likely(x)      __builtin_expect(!!(x), 1)               //!!(x) 得到x的布尔值.
#define unlikely(x)    __builtin_expect(!!(x), 0)


int main(int argc,char **argv)
{
    int a,b;
    a = atoi(argv[1]);
    b = atoi(argv[2]);

    int e = (a ?: b);		    //gnu extention for c. ref using-GCC_Using the GNU.... 6.7
    int c = (a==2 ?: b);
    int d = (a!=2 ?: b);

    printf("a:%d,b:%d,e:%d,c:%d,d:%d\n",a,b,e,c,d);

    if (likely(a == 2)){
        printf("likely a=2\n");
    } else {
        printf("likely a!=2\n");
    }

    if (b){
        printf("b!=0\n");
    } else {
        printf("b==0\n");
    }

    if (likely(b)){
        printf("likely b!=0\n");
    } else {
        printf("likely b==0\n");
    }

    if (unlikely(b)){
        printf("unlikely b!=0\n");
    } else {
        printf("unlikely b==0\n");
    }

    if (__builtin_expect(b,1)){
        printf("__builtin_expect(b,1)\n");
    } else {
        printf("__builtin_expect(!b,1)\n");
    }
    return 0;
}
