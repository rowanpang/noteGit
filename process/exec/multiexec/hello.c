#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void show(char* msg)
{
    printf("%s\n",msg);
}
int secwait=3;

char *msg="pagnwz";
int main(int argc,char **argv)
{
    /*change to linebuf mode,so no need fource flush*/
    /*setlinebuf(stdout);*/
    printf("hello argc :%d\n",argc);
    if (argc=2){
	secwait = atoi(argv[1]);
    }

    printf("hello world,sleep %d\n",secwait);
    sleep(secwait);
    return 0;
}
