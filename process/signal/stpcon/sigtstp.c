#include <stdio.h>
#include <unistd.h>

int main(int argc,char** argv)
{
    int i;
    printf("do{print(); sleep(1);}while(1) pid:%d\n",getpid());

    while(1){
        printf("hello world %d\n",i++);
        sleep(3);
    };

    return 0;
}
