#include<stdio.h>
#include<unistd.h>

int main(int argc,char** argv)
{
    int a = 100;
    int b = 0;
    int c;
    char *pb=0;
    
    printf("cur pid:%d\n",getpid());

    *pb = 1;
    c = a/b;

    return 0;
}
