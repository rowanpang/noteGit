#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <unistd.h>

int count = 0;
int num = 5;
int main(int argc,char** argv)
{
    int ret;
    pid_t child_pid;

    if(argc >=2){
        num = atoi(argv[1]);
        printf("arg num:%d\n",num);
    }
    printf("num:%d\n",num);

    int *t = malloc(sizeof(int));

    ret = fork();
    switch(ret){
	case 0:		    //child

	    break;
	case -1:
	    ret = -1;
	    return ret;
	default:	    //parent
	    child_pid = ret;
	    ret = 0;
	    printf("in parent child pid:%d\n",child_pid);
	    return ret;
    }

    /*setsid();*/
    /*signal(SIGCHLD, SIG_IGN);*/
    /*signal(SIGHUP, SIG_IGN);*/

    chdir("/");
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--) {
	close (x);
    }

    openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
    while (1) {
	syslog (LOG_NOTICE, "First daemon started.");
	sleep (20);
	/*break;*/
    }

    return 0;
}
