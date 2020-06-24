#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>


typedef _Bool bool;

#define TMPFILE "/tmp/rowanflock.lock"

int main(int argc,char **argv)
{
    int ret=0;
    int fd;
    char buf[32];

    int i;
    printf("pid:%d,tmpfile:%s\n",getpid(),TMPFILE);

    printf("flags O_DIRECT:%#x,O_RDONLY:%#x\n",O_DIRECT,O_RDONLY);
    ret = open(TMPFILE,O_RDWR |O_CREAT ,S_IRUSR | S_IWUSR);
    if (ret == -1) {
	perror("open target dev error");
	ret = errno;
	goto OUT;
    }
    fd = ret;

    /*
     *printf("input a number and try to lock the file: \n");
     *scanf("%d",&i);
     */

    if(flock(fd,LOCK_EX | LOCK_NB)==0) {
	printf("locking success\n");
	goto PAUSE;
    } else {
	if (errno == EWOULDBLOCK) {	//equal to EAGAIN
	    printf("has been locked, cmd lslocks to check:%d,%s\n",errno,strerror(errno));
	} else {
	    printf("locking error:%d,%s\n",errno,strerror(errno));
	}
	goto CLOSE;
    }

PAUSE:
    pause();

CLOSE:
    close(fd);
OUT:
    return ret;
}
