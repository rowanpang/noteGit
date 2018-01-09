#include<stdio.h>
#include<unistd.h>
#include<errno.h>
int doCChild()
{
    int i;
    for (i = 0; i < 5; ++i) {
	printf("Im cchild\n");
	sleep(2);
    }
}

int doChild()
{
    int i;
    int ccpid;
    ccpid = fork();
    if (ccpid == 0){
	/*child*/
	doCChild();
    }else if (ccpid > 0){
	/*parent*/
	printf("Im parent for ccpid:%d\n",ccpid);
	sleep(5);
    }
}

int main(int argc,char** argv)
{
    int cpid;

    printf("aeon:%d\n",getpid());
    cpid = fork();
    if (cpid == 0){
	/*child*/
	doChild();
    }else if (cpid > 0){
	/*parent*/
	printf("Im parent for cpid:%d\n",cpid);
	sleep(20);
    }else{
	perror("when do fork");
    }

    printf("pid:%d,exit\n",getpid());

    return 0;
}

