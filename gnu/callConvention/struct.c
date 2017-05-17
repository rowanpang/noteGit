#include<stdio.h>

float floatRet(){
    return 2.2;
}

long longRet(){
    return 2;
}

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
    long retLong;
    float retfloat;
    ret = hello(1,2,3);
    retLong = longRet();
    retfloat = floatRet();

    return ret;
}

