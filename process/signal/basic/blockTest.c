#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc,char **argv)
{
    double y;
    sigset_t intmask;
    int i,repeat_factor;
    if(argc!=2){
	fprintf(stderr,"Usage:%s repeat_factor\n\a",argv[0]);
	exit(1);
    }

    if((repeat_factor=atoi(argv[1]))<1){
	repeat_factor=10;
    }

    sigemptyset(&intmask);
    sigaddset(&intmask,SIGINT);

    while(1){
	sigprocmask(SIG_BLOCK,&intmask,NULL);
	fprintf(stderr,"SIGINT signal blocked\n");
	sleep(10);

	fprintf(stderr,"SIGINT signal will unblocked\n");
	/*signal will deliver util unblock*/
	sigprocmask(SIG_UNBLOCK,&intmask,NULL);
	fprintf(stderr,"SIGINT signal unblocked\n");
	sleep(5);
    }
    exit(0);
}
