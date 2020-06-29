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
    struct flock fltmp;

    int ret;

    fd = open(FILENAME,O_RDWR | O_CREAT,644);

    //getlk 相同的地方
    fltmp.l_type = F_RDLCK;
    fltmp.l_start = 0;
    fltmp.l_whence = SEEK_SET;
    fltmp.l_len = 1;

    ret = fcntl(fd,F_GETLK,&fltmp);
    if (ret !=0 ){
	printf("fcntl getlk error:%d,%s\n",errno,strerror(errno));
	goto OUT;
    } else {
	printf("fcntl getlk success flock ret:%d\n",fltmp.l_type);
	/*
	 *#define F_RDLCK         0
	 *#define F_WRLCK         1
	 *#define F_UNLCK         2
	 */
    }

    //getlk 不同的地方
    fltmp.l_type = F_RDLCK;
    fltmp.l_start = 5;
    fltmp.l_whence = SEEK_SET;
    fltmp.l_len = 1;

    ret = fcntl(fd,F_GETLK,&fltmp);
    if (ret !=0 ){
	printf("fcntl getlk error:%d,%s\n",errno,strerror(errno));
	goto OUT;
    } else {
	printf("fcntl getlk success flock ret:%d\n",fltmp.l_type);
	/*
	 *#define F_RDLCK         0
	 *#define F_WRLCK         1
	 *#define F_UNLCK         2
	 */
    }

    if(write_lock(fd,0,SEEK_SET,5) < 0) {
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
    close(fd);
    return 0;
}
