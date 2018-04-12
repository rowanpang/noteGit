#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <sys/epoll.h>
#include <sys/socket.h>

int showMsg(int arg1,int arg2,int arg3)
{
    printf("Testing %d, %d, %d!\n", arg1, arg2, arg3);

    return 0;
}

int main(int argc, char *argv[])
{
    int one = 1, two = 2, three = 3;
    printf("Testing %lx, %lx, %lx!\n");
    showMsg(one,two,three);

    return 0;
}
