#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>


int main(int argc,char* argv[])
{
    pid_t pid;
    int i;

    while(1){
        pid=getpid();
        usleep(1);
    }

    return 0;
}
