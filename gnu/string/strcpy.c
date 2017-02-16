#include<stdio.h>
#include<string.h>

int main(int argc,char** argv)
{
    char *src="pangwz";
    char dst[10]={0};

    strcpy(dst,src);

    printf("after cpy %s \n",dst);

    return 0;
}
