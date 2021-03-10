#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>

#include<sys/epoll.h>
#include<sys/types.h>
#include<sys/syscall.h>


#define MAXEVENTS   1
#define READMAX     10

int main(int argc, char** argv)
{
    int fd=0;
    int efd;
    int ret;
    int i;
    struct epoll_event event;
    struct epoll_event* events;
    char buf[READMAX];

    efd = epoll_create1(0);
    if (efd == -1){
        perror ("epoll_create");
        abort ();
    }

    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;

    ret = epoll_ctl(efd,EPOLL_CTL_ADD,fd,&event);
    if (ret == -1){
        perror("epoll_ctl");
        abort ();
    }

    events = calloc(MAXEVENTS, sizeof(struct epoll_event));
    printf("epoll_wait stdin, press enter to continue...\n");
    ret = epoll_wait (efd, events, MAXEVENTS, -1);
    printf("epoll_wait ret:%d,errno:%d\n",ret,errno);
    for(i=0; i<ret; i++){
        printf("evt flg for events %d is %x\n",i,events[i].events);
    }
    ret = read(events[0].data.fd,buf,READMAX);
    printf("read up %d bytes,wait thread\n",ret);

    return 0;
}
