#include<stdio.h>
#include<sched.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int func_clone(void *arg)
{
	printf("in %s\n",__FUNCTION__);
	execl("/bin/bash",NULL);
	return 0;
}

int main(int argc,char **argv)
{
	int childId = 0;
	int status = 0;
	int ss = 4*getpagesize();
	void *cs,*s = malloc(ss);
	cs = s + ss -1;

	childId = clone(func_clone,cs,0,NULL);

	//sleep(3);
	waitpid(childId,&status,__WALL);
	printf("in main %d\n",childId);
	
		
	return 0;
}
