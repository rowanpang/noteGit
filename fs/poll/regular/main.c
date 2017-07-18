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
#include<poll.h>

typedef _Bool bool;

int main(int argc,char **argv)
{
    int ret=0;
    int fd;
    char buf[32];
    printf("pid:%d\n",getpid());
    ret = open("/tmp/pollRowan.txt",O_RDWR |O_CREAT,S_IRUSR | S_IWUSR);
    if (ret == -1) {
	perror("open target dev error");
	ret = errno;
	goto OUT;
    }
    fd = ret; 
    lseek(fd,0,SEEK_SET);
    
    ret = write(fd,"aaaaaaaadddddddddddaaaaaa",10);
    if (ret == -1) {
	perror("when write");
	ret = errno;
	goto CLOSE;
    }else{
	printf("write bytes:%d\n",ret);
    }

    struct pollfd fds[1]={
	{
	    .fd = fd,
	    .events = POLLIN,
	},
    };

    printf("fds num:%d\n",sizeof(fds)/sizeof(struct pollfd));
    ret = poll(fds,sizeof(fds)/sizeof(struct pollfd),-1);
    printf("poll return :%d\n",ret);
    printf("poll revents :%d\n",fds[0].revents);

    lseek(fd,0,SEEK_SET);
    ret = read(fd,buf,32);
    if (ret == -1) {
	perror("when read");
	ret = errno;
	goto CLOSE;
    }else{
	printf("read bytes:%d\n",ret);
    }

CLOSE: 
    close(fd);
OUT:
    return ret;
}
