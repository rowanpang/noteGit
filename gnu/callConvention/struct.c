#include<stdio.h>

int hello(int ok){
    int doit;
    int a;
    doit = ok;
    a = 3;
    printf("hello from the other side\n");
}

int main(int argc,char** argv)
{
    hello(1);
    return 0;
}

