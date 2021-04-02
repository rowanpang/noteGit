#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
/*
 *1,运算时先统一类型,再执行操作.
 *2,char/uchar -> int -> uint -> long -> ulong -> ll -> ull -> float -> double -> udouble
 *3,负数,采用补码表示 https://en.wikipedia.org/wiki/Signed_number_representations
 */
int main(int argc,char** argv)
{
    {
        unsigned char a,b,c;
        unsigned char base=10;
        char cmp1,cmp2,cmp3,cmp4,cmp5,cmp6,cmp7,cmp8;
        unsigned char cmp11,cmp12,cmp13,cmp14;
        char c2;
        b = 9;
        a = 2;

        c = a - b ;                     //c=256-b+a
        c2 = a - b ;

        cmp1 = a - b < 10;              //cmp1=1; (a-b)的结果为uchar, 再与10比较, 10默认是int, uchar 隐式转为int 比较-7<10;
        cmp2 = a - b < base;
        cmp3 = c < 10;                  //cmp3=0; c是uchar, 用uchar解析内存数值>10.
        cmp4 = c < base;                //cmp4=0;

        cmp5 = 10 > a - b;              //同上.
        cmp6 = base > a - b;            //..
        cmp7 = 10 > c;                  //..
        cmp8 = base > c;                //..

        cmp11 = a - b < 10;
        cmp12 = a - b < base;
        cmp13 = c2 < 10;                //c2,为char, -7 < 10,　cmp13=1.
        cmp14 = c2 < base;

        printf("char udiff c:%u,diff c2:%d\n",c,c2);
        printf("cmp1:%d,cmp2:%d,cmp3:%d,cmp4:%d,cmp5:%d,cmp6:%d,cmp7:%d,cmp8:%d\n",cmp1,cmp2,cmp3,cmp4,cmp5,cmp6,cmp7,cmp8);
        printf("cmp11:%u,cmp12:%u,cmp13:%u,cmp14:%u\n",cmp11,cmp12,cmp13,cmp14);
    }

    printf("\n--------\n\n");
    {
        unsigned char a,b,c;
        unsigned char base=10;
        char cmp1,cmp2,cmp3,cmp4,cmp5,cmp6,cmp7,cmp8;
        unsigned char cmp11,cmp12,cmp13,cmp14;
        char c2;
        b = 254;
        a = 2;

        c = a - b ;                     //c=256-b+a
        c2 = a - b ;

        cmp1 = a - b < 10;              //cmp1=1; (a-b)的结果为uchar, 再与10比较, 10默认是int, uchar 隐式转为int 比较-7<10;
        cmp2 = a - b < base;
        cmp3 = c < 10;                  //cmp3=0; c是uchar, 用uchar解析内存数值>10.
        cmp4 = c < base;                //cmp4=0;

        cmp5 = 10 > a - b;              //同上.
        cmp6 = base > a - b;            //..
        cmp7 = 10 > c;                  //..
        cmp8 = base > c;                //..

        cmp11 = a - b < 10;
        cmp12 = a - b < base;
        cmp13 = c2 < 10;                //c2,为char, -7 < 10,　cmp13=1.
        cmp14 = c2 < base;

        printf("char udiff c:%u,diff c2:%d\n",c,c2);
        printf("cmp1:%d,cmp2:%d,cmp3:%d,cmp4:%d,cmp5:%d,cmp6:%d,cmp7:%d,cmp8:%d\n",cmp1,cmp2,cmp3,cmp4,cmp5,cmp6,cmp7,cmp8);
        printf("cmp11:%u,cmp12:%u,cmp13:%u,cmp14:%u\n",cmp11,cmp12,cmp13,cmp14);
    }

    printf("\n--------\n\n");

    {
        unsigned int a,b,c;
        unsigned int base=10;
        int cmp1,cmp2,cmp3,cmp4;
        unsigned int cmp11,cmp12,cmp13,cmp14;
        int c2;
        b = 9;
        a = 2;

        c = a - b ;
        c2 = a - b ;

        cmp1 = a - b < 10;              //cmp1=0;统一为unsigned int 再比较.cmpX == 0;
        cmp2 = a - b < base;
        cmp3 = c < 10;                  //cmp3=0;c uint, 准为uint比较.
        cmp4 = c < base;

        cmp11 = a - b < 10;             //cmp11=0; (a-b),结果uint,转为uint比较
        cmp12 = a - b < base;
        cmp13 = c2 < 10;                //cmp13=1; c2 int, 10 int
        cmp14 = c2 < base;              //cmp14=0; c2 int, base uint, 转为uint

        printf("unsinged int diff c:%u,c2:%d\n",c,c2);
        printf("cmp1:%d,cmp2:%d,cmp3:%d,cmp4:%d\n",cmp1,cmp2,cmp3,cmp4);
        printf("cmp11:%u,cmp12:%u,cmp13:%u,cmp14:%u\n",cmp11,cmp12,cmp13,cmp14);
    }

    return 0;
}

