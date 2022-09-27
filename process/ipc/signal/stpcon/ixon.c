#include <stdio.h>
#include <unistd.h>

int main(int argc,char** argv)
{
    int i;
    printf("do while(1) sleep(1),pid:%d\n",getpid());

    while(1){
        sleep(2);
    };

    return 0;
}
