#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc,char** argv)
{
    int ret;
    int cstatus;
    pid_t cpid;    

    printf("in main task pid: %d\n",getpid());
    cpid = fork();

    switch(cpid){
	case 0:
	    printf("in child \n");
	    sleep(100);
	    break;
	    
	case -1:
	    perror("when fork");
	    ret = -1;
	    goto out;

	default:
	    printf("child pid %d\n",cpid);
	    wait(&cstatus);
	    sleep(100);
	    printf("child exit %d\n",cstatus);
	    break;
    }
    
    ret = 0;
out:
    printf("exit\n");
    return ret;
}
