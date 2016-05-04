#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int count = 0;
int num = 5;
int main(int argc,char** argv)
{
	pid_t child_pid;

	if(argc >=2){
		num = atoi(argv[1]);
		printf("arg num:%d\n",num);
	}
	printf("num:%d\n",num);

	int *t = malloc(sizeof(int));

	child_pid=fork();
	if(child_pid){
		//parent
		printf("parent waitpid %d\n",child_pid);
		return 0;
		waitpid(child_pid,NULL,0);
		printf("parent after waitpid\n");
	}else{
		//child
		//setsid();
		sleep(30);
	}

	return 0;
}
