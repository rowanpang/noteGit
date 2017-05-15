#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <error.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

int main(int argc,char **argv)
{
    int ret;
    int master;
    char *slavePath;
    int slave,slaveAgain;
    ret = open("/dev/ptmx",O_RDWR|O_NOCTTY);	//equal to getpt()
    if(ret == -1){
	perror("ptmx open error");
	goto OUT;
    }
    master = ret;

    ret = grantpt(master);
    if(ret != 0){
	perror("grantpt error!");
	goto CLOSE_MASTER;
    }
    ret = unlockpt(master);
    if(ret != 0){
	perror("unlockpt error!");
	goto CLOSE_MASTER;
    }

    slavePath = ptsname(master);    
    printf("new ptmx paire: %d,%s\n",master,slavePath);
    ret = open(slavePath,O_RDWR);
    if(ret == -1){
	perror("slave open error");
	goto CLOSE_MASTER;
    }
    slave = ret;
    printf("new ptmx paire: %d,%d\n",master,slave);

    struct termios oterm;
    ret = tcgetattr(slave,&oterm);
    printf("fd:%d, c_lflag:%#x\n",slave,oterm.c_lflag);

    ret = open(slavePath,O_RDWR);
    if(ret == -1){
	perror("slave open error");
	goto CLOSE_MASTER;
    }
    slaveAgain = ret;
    ret = tcgetattr(slaveAgain,&oterm);
    printf("fd:%d, c_lflag:%#x\n",slaveAgain,oterm.c_lflag);

    sleep(40);

    ret = 0;
CLOSE_SLAVE:
    close(slave);
CLOSE_MASTER:
    close(master);
OUT:
    return ret;
}
