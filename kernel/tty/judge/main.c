#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<linux/kd.h>

int isvt(int fd)
{
    char arg;	//OR int arg=0;	for kernel just valued 1 byte.if not init to 0, arg value is random.
    return (isatty (fd) && ioctl(fd, KDGKBTYPE, &arg) == 0
                && ((arg == KB_101) || (arg == KB_84)));
}


int main(int argc,char **argv)
{
    int ret=0;
    int fd;
    char *devpath="/dev/tty";

    if (argc > 1){
	devpath = argv[1];
    }

    ret = open(devpath,O_RDONLY);
    if (ret == -1) {
        perror("open target dev error");
        ret = errno;
        goto OUT;
    }
    fd = ret; 
    
    printf("fd:%s,isatty:%d\n",devpath,isatty(fd));
    printf("fd:%s,isvt:%d\n",devpath,isvt(fd));

OUT:
    return ret;
}
