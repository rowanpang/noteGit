#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int show(int isShow)
{
	if(isShow)
		printf("in show\n");

	return 0;
}

int gVar = 10;
int gVarUninit;

/*初始化的var 将增加elf文件的size.*/
/*int gVarArrary[1024*1024];*/
/*int gVarArrary2[1024*1024]={1};*/
int main(int argc,char **argv)
{
	int ret = 0;
	static int sVar = 19;
	static int sVarUninit;
	printf("pid %d\n",getpid());
	printf("in main arg[0]:%s %#x\n",argv[0],&argv[0]);
	printf("in main sVar:%#x=%d,gVar:%#x=%d \n",sVar,sVar,gVar,gVar);
	
	show(1);
	int second = 30;
	printf("start sleep %ds\n",second);
	sleep(second);

	return ret;
}
