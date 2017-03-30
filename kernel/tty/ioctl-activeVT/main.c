#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<linux/kd.h>
#include<linux/vt.h>

typedef _Bool bool;

/*
 *usage:
 *    arg1: int X: change to vt x defautl is 3.
 */
int main(int argc,char **argv)
{
    int ret=0;
    int fd;
    int ttyX=1;
#define DEVPREFIX "/dev/tty"
    char devpath[20];
    int vtTarget=3;

    if (argc >1){
	vtTarget = atoi(argv[1]);
    }
    printf("target vt is %d\n",vtTarget);
    sprintf(devpath,"%s%s",DEVPREFIX,"0"); //printf("target dev is %s\n",devpath);

    ret = open(devpath,O_RDONLY);
    if (ret == -1) {
	perror("open target dev error");
	ret = errno;
	goto OUT;
    }
    fd = ret; 
    
    ret = ioctl(fd,VT_ACTIVATE,vtTarget);
    if (ret == -1) {
	perror("error for VT_ACTIVATE");
	ret = errno;
	goto CLOSE;
    }else{
	printf("switch to vt:%d\n",vtTarget);
    }

CLOSE: 
    close(fd);
OUT:
    return ret;
}
