#include<stdio.h>
//#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<unistd.h>

//pangwz
//	if arg... is none,'##'remove the pre ',',so dprintf("---\n") is ok!!! and '..,st' is also ok!!!
#define dprintf(fmt,arg...) printf("pangwz"fmt,##arg)

//pangwz,
//	after spread out there is a ',' so need one more arg,just dprintf("---\n") is error!!!.need dprintf("---\n",st);
//#define dprintf(fmt,arg...) printf("pangwz "fmt,arg)
//#define dprintf(fmt,...) printf("pangwz "fmt,__VA_ARGS__)

int main(int argc,char** argv)
{
        printf("%ld\n",sysconf(_PC_PATH_MAX));
        printf("%d\n",PATH_MAX);
	
		dprintf("----\n");

		char realPath_l[4097]={0};
		char *p=NULL;
		p = realpath(argv[1],&realPath_l[0]);
		printf("p:%p, path:%s , realPath:%s\n",p,argv[1],realPath_l);

        return 0;
}
