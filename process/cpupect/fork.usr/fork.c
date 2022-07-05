#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>

int childproc(int fd)
{
    while(1);
    return 0;
}

int main(int argc,char* argv[])
{
    pid_t pid;
    int i;

    pid=fork();
    if(pid==0) {                    /*child*/
        childproc(0);
        exit(0);
    } else if(pid>0) {              /*parent*/
        printf("ppid:%d,pid:%d\n",getpid(),pid);
        while(1);
    }

    return 0;
}
