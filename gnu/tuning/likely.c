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

    if (likely(a)){
        printf("likely a\n");
    } else {
        printf("likely !a\n");
    }

    if (unlikely(b)){
        printf("unlikely b\n");
    } else {
        printf("unlikely !b\n");
    }

    return 0;
}
