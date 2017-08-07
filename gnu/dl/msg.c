#include <stdio.h>
#include <string.h>

/*int printf (const char *__restrict __format, ...)*/
/*{*/
    /*puts("by prload so\n");*/
    /*puts(__format);*/
    /*return 0;*/
/*}*/


int strcmp(const char* s1,const char* s2)
{
    printf("preload invoke\n");
    return 0; 
}

int showmsg(char* msg)
{
    puts("in msg.so func showmsg:");
    puts(msg);
    puts("\n");
}
