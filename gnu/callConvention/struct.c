#include<stdio.h>

int hello(int ok,int arg2,int arg3){
    int doit;
    int a,b;
    doit = ok;
    a = arg2;
    b = arg3;
    printf("hello from the other side\n");
    return 2;
}

int main(int argc,char** argv)
{
    int ret;
    ret = hello(1,2,3);
    return ret;
}

