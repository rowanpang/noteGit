#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char **argv)   
{   
    int ret;   
    char buf[20];
    
    bzero(buf,20);
    /*write(STDOUT_FILENO,"before read\n",15);*/
    printf("before read\n");
    ret = read(STDIN_FILENO,buf,sizeof(buf));
    printf("readin--1:%s\n",buf);
    sleep(3);
    /*ret = read(STDIN_FILENO,buf,sizeof(buf));*/
    printf("readin--2:%s\n",buf);

    ret = 0;
    return ret;   
} 
