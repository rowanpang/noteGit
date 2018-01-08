#include <stdio.h>
#include <unistd.h>

void show(char* msg)
{
    printf("%s\n",msg);
}

char *msg="pagnwz";
int main(int argc,char **argv)
{
    printf("hello world\n");
    show("msg");
    fflush(stdout);
    sleep(10);
    return 0;
}
