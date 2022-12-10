#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <linux/kd.h>
#include <linux/vt.h>

typedef _Bool bool;

int main(int argc,char **argv)
{
    int ret=0;
    int fd;

    printf("pid:%d,tmpfile:/tmp/rowan.txt\n",getpid());
    ret = open("/tmp/rowan.txt",O_RDWR |O_CREAT ,S_IRUSR | S_IWUSR);
    if (ret == -1) {
	perror("open target dev error");
	ret = errno;
	goto out;
    }
    fd = ret;

    printf("pause,wait for signal\n",getpid());
    pause();

    printf("after pause, close file\n");
    close(fd);
out:
    return ret;
}
