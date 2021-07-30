#include <stdio.h>
#include <stdlib.h>

int myadd9(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)\
{
    int sum;
    sum = a1+a2+a3+a4+a5+a6+a7+a8+a9;

    return sum;
}

int myadd(int a, int b)
{
    int c;
    int d;

    c = random();

    d = a + b + c;
    return d;
}

int main()
{
    int a;
    int b;
    int aa1,aa2,aa3,aa4,aa5,aa6,aa7,aa8,aa9;
    int sum,sum1,sum2;

    a = random();
    b = random();
    sum = a + b;
    sum1 = myadd(a,b);

    aa1 = random();
    aa2 = random();
    aa3 = random();
    aa4 = random();
    aa5 = random();
    aa6 = random();
    aa7 = random();
    aa8 = random();
    aa9 = random();

    sum2 = myadd9(aa1,aa2,aa3,aa4,aa5,aa6,aa7,aa8,aa9);

    return 0;
}


