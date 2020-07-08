#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int mymsg(const char* msg,int cnt);

int main(int argc,char** argv)
{
    char* pwd = "askdf";
    char cmd[128];

    sprintf(cmd,"cat /proc/%d/maps",getpid());
    printf("cmd:%s, pid:%d,%#x\n",cmd,getpid(),getpid());
    system(cmd);
    printf("\n\n");

    printf("func mymsg ptr:%p\n",mymsg);
    mymsg("hello world",getpid());

    /*sleep(300);*/

    return 0;
}
