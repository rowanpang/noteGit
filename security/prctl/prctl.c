#include <stdio.h>
#include <sys/prctl.h>

int main(int argc,char** argv)
{
	int ret;
	
	ret=prctl(PR_GET_SECUREBITS,0,0,0,0);	

	printf("ret:%#x\n",ret);

	return 0;	
}
