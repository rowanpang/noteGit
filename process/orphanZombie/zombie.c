#include<stdio.h>
#include<unistd.h>
#include<errno.h>

int main(int argc,char** argv)
{
    int cpid;

    cpid = fork();
    if (cpid == 0){
	printf("Im child\n");
    }else if (cpid > 0){
	/*parent*/
	printf("Im parent,cpid:%d\n",cpid);
	sleep(15);
	/*child*/
    }else{
	perror("when do fork");
    }

    return 0;
}

