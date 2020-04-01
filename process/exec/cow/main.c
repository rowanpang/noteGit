#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>


int pipefd[2];
int all=5;

int childrun()
{
    int ret;
    char *args[] = {"bashHello","-i",NULL};
    char buf[5];
    close(pipefd[1]);
    printf("childrun before read\n");
    read(pipefd[0],buf,4);	//"sync"

    printf("all %d\n",all);
    sleep(2);
    printf("all2 %d\n",all);


    printf("childrun do execv\n");
    ret = execv("./hello",args);

    perror("when execv");   //should not be here;
    return ret;
}

int cpids[1024];

int forknum=5;


int main(int argc,char** argv)
{
    int ret;
    int i;
    pid_t cpid;
    pipe(pipefd);

    for(i = 0; i < forknum ; i++){
	printf("forkchild :%d, ",i);
	cpid = fork();
	if(!cpid){
	    /*child*/
	    childrun();
	}else if(cpid != -1){
	    cpids[i] = cpid;
	    printf("cpid: %d\n",cpid);
	}else{
	    perror("when fork");
	    ret = -1;
	    goto out;
	}
    }

    printf("all update\n");
    all=3;

    close(pipefd[0]);
    usleep(1000*500);
    for(i = 0; i < forknum ; i++){
	printf("write pipe %d\n",i);
	write(pipefd[1],"sync",4);
    }
    sleep(5);
    kill(0,SIGINT);

    int cstatus;

    for(i = 0; i < forknum ; i++){
	printf("starting wait child :%d\n",i);
	waitpid(cpids[i],&cstatus,0);
	printf("child exit pid:%d,%d\n",cpids[i],cstatus);
    }

    ret = 0;
out:
    printf("father exit\n");
    return ret;
}
