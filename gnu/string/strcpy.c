#include<stdio.h>
#include<string.h>

int main(int argc,char** argv)
{
    char *src="pangwz";
    char dst[10]={0};

    char *src2="123";

    src="kkkk";
    printf("%02x,%02x,%02x\n",'1',src2[0],src2[1]);
    printf("%02x,%02x,%02x\n",'1',*src2++);
    printf("%02x,%02x,%02x\n",'1',*src2++);

    strcpy(dst,src);

    printf("after cpy %s \n",dst);

    return 0;
}
