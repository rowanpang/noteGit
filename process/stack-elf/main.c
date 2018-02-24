#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int show(int isShow)
{
    int arr[1024];
    if(isShow)
	printf("in show\n");

    return 0;
}

int gVar = 10;
int gVarUninit;

/*.bss segment*/
int gVarArrary[1024*1024];

/*初始化的var 将增加elf文件的size., in .data segment*/
unsigned int gVarArrary2[2*1024]={
    [0 ... 1023] = 0x11223344,
    0
};

int main(int argc,char **argv)
{
    /*init val 直接体现在code中 mov 0x05 addr*/
    int ret = 0x05;

    /*.rodata 中'zhelloPangwz' 地址入栈*/
    char *hello = "zhelloPangwz";

    /*initliazed static var in .data segment*/
    static int sVar = 19;
    /*uninit in .bss segment*/
    static int sVarUninit;

    printf("pid %d\n",getpid());
    printf("in main arg[0]:%s %#x\n",argv[0],&argv[0]);
    printf("in main sVar:%#x=%d,gVar:%#x=%d \n",sVar,sVar,gVar,gVar);

    char *msg = NULL;

    /*heap*/
    msg = malloc(2*1024);

    memcpy(msg,hello,strlen(hello));

    show(1);
    int second = 30;
    printf("start sleep %ds\n",second);
    sleep(second);

    free(msg);

    return ret;
}
