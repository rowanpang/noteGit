#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

void sigchldHandle(int sig)
{
    siginfo_t info;

    if (sig != SIGCHLD){
	return;
    }

    waitid(P_ALL,0,&info,WEXITED);
    printf("after reap,child info pid:%d\n",info.si_pid);
}

int main(int argc,char** argv)
{
    int cpid;

    cpid = fork();
    signal(SIGCHLD,sigchldHandle);
    if (cpid == 0){
	/*child*/
	printf("Im child\n");
	sleep(1);
    }else if (cpid > 0){
	/*parent*/
	printf("Im parent,cpid:%d\n",cpid);
	sleep(5);	//will be wakeup soon
	printf("signal wakeup\n");
	sleep(20);
    }else{
	perror("when do fork");
    }

    printf("pid %d,exit\n",getpid());

    return 0;
}

