#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <string.h>

#define write_lock(fd,offset,whence,len)\
		lock_reg(fd,F_SETLK,F_WRLCK,offset,whence,len)

#define FILENAME "/tmp/rowanposixlock.lock"


int lock_reg(int fd,int cmd,int type,off_t offset,int whence,off_t len)
{
    struct flock lock;
    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    return (fcntl(fd,cmd,&lock));
}

int main(int argc, char const *argv[])
{
    int fd;
    char line[100];
    fd = open(FILENAME,O_RDWR | O_CREAT,644);
    if(write_lock(fd,0,SEEK_SET,0) < 0) {
	if(errno == EACCES || errno == EAGAIN){	//EAGAIN == EWOULDBLOCK
	    printf("unable to lock %s,is %s already running?\n", FILENAME,argv[0]);
	    goto OUT;
	} else {
	    printf("unable to lock %s,error:%d,%s",FILENAME,errno,strerror(errno));
	}
    }

    printf("locking success\n");

    snprintf(line,sizeof(line),"%ld\n",(long)getpid());
    ftruncate(fd,0);
    write(fd,line,strlen(line));    //sizeof和strlen的区别也是需要注意的

    pause();


OUT:
    return 0;
}
