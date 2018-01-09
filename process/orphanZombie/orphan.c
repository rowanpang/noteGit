#include<stdio.h>
#include<unistd.h>
#include<errno.h>

int doChild()
{
    int i;
    for (i = 0; i < 10; ++i) {
	printf("Im child\n");
	sleep(2);
    }
}

int main(int argc,char** argv)
{
    int cpid;

    cpid = fork();
    if (cpid == 0){
	/*child*/
	doChild();
    }else if (cpid > 0){
	/*parent*/
	printf("Im parent,cpid:%d\n",cpid);
    }else{
	perror("when do fork");
    }

    return 0;
}

