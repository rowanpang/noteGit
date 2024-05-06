#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv)
{
    int a,b,c;
    char *dest,*src;

    b=a;
    c=1;
    b=c;

    b=atoi(argv[1]);

    dest = malloc(128);
    src = malloc(128);

    memset(src,b,16);
    memcpy(dest,src,16);

    printf("dest[0]:%ld\n",dest[0]);

    free(dest);
    free(src);
}
