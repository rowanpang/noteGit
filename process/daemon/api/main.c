#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <unistd.h>

int main(int argc,char** argv)
{
    int ret;

    openlog("firstdaemon", LOG_PID, LOG_DAEMON);
    printf("main task: %d\n",getpid());

    daemon(1,1);
    printf("task pid after daemon(): %d\n",getpid());
    while (1) {
	syslog (LOG_NOTICE, "--in daemon task--.");
	sleep (2);
    }

    return 0;
}
