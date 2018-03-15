#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <pty.h>

int main(int argc,char **argv)
{
    int ret;
    int ptm;
    char ptsName[PATH_MAX];
    pid_t cpid;

    ret = forkpty(&ptm,ptsName,NULL,NULL);

    if (ret  == -1){
	perror("when fork");
	ret = errno;
	goto OUT;
    }else if (ret == 0){
	/*child process*/
	execl("/bin/bash", "bash", NULL);
    }else {
	/*parent process*/
	cpid = ret;
	printf("cpid:%d\n", cpid);

	waitpid(cpid,NULL,0);
    }


    ret = 0;

OUT:
    return ret;
}
