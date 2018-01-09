#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(int argc,char** argv)
{
    int cpid;
    siginfo_t info;

    cpid = fork();
    if (cpid == 0){
	/*child*/
	printf("Im child\n");
    }else if (cpid > 0){
	/*parent*/
	printf("Im parent,cpid:%d\n",cpid);
	sleep(5);
	waitid(P_ALL,0,&info,WEXITED);
	printf("after reap,child info pid:%d\n",info.si_pid);
	sleep(5);
    }else{
	perror("when do fork");
    }

    printf("pid %d,exit\n",getpid());

    return 0;
}

