#include <stdio.h>
#include <unistd.h>

void show(char* msg)
{
    printf("%s\n",msg);
}

char *msg="pagnwz";
int main(int argc,char **argv)
{
    /*change to linebuf mode,so no need fource flush*/
    setlinebuf(stdout);
    printf("hello world,sleep 3\n");
    /*fflush(stdout);*/
    sleep(3);
    show("sleep 2 seconds");
    /*fflush(stdout);*/
    sleep(2);
    return 0;
}
