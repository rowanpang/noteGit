#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int count = 0;
int num = 1;
int main(int argc,char** argv)
{
    pid_t child_pid;
    char buf[100]="";
    int ret;

    if(argc >=2){
        num = atoi(argv[1]);
        printf("arg num:%d\n",num);
    }
    printf("num:%d\n",num);

    int *t = malloc(sizeof(int));

    child_pid=fork();
    if(child_pid){
        //parent
	printf("-----------in parent-----------\n");
        while(count++ < num){
            (*t)++;
            printf("pid:%d\n",(int)getpid());
            printf("pgrp:%d\n",(int)getpgrp());
            printf("tcpgrp:%d\n",(int)tcgetpgrp(STDIN_FILENO));
            printf("count:%d\n",count);
            printf("t:%d\n",*t);
        }

	/*read(STDIN_FILENO,buf,30);*/
	printf("readin:%s\n",buf);

	sleep(1);
        waitpid(child_pid,NULL,0);
        printf("parent after waitpid %d\n",child_pid);

    }else{
        //child
	printf("-----------in child-----------\n");
	setpgrp();
        while(count++ < num){
            (*t)++;
            printf("pid:%d\n",(int)getpid());
            printf("pgrp:%d\n",(int)getpgrp());
            printf("tcpgrp:%d\n",(int)tcgetpgrp(STDIN_FILENO));
            printf("count:%d\n",count);
            printf("t:%d\n",*t);
        }
	ret=tcsetpgrp(STDIN_FILENO,getpgrp());
	printf("tcsetpgrp:%d\n",ret);
	/*read(STDIN_FILENO,buf,30);*/
	printf("readin:%s\n",buf);
	sleep(10);
    }

    return 0;
}
