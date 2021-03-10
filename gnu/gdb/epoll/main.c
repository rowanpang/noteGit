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

#define gettid() syscall(__NR_gettid)

pthread_barrier_t barrier;
void* threadfn2(void *arg)
{
    int ret;
    int i = 0;

    printf("in thread:%d\n",gettid());
    while(i++ < 5){
        printf("hello world: %d\n",i);
        sleep(1);
    }

    ret = 0;

    printf("out thread:%d\n",gettid());
    return NULL;
}

void* threadfn(void *arg)
{
    int ret;
    pthread_t th2;
    void* th2ret;

    printf("in thread:%d\n",gettid());
    pthread_barrier_wait(&barrier);     //wait for main process, ensure create thread after epoll;
    sleep(2);

    ret = pthread_create(&th2,NULL,threadfn2,NULL);
    if (ret){
        perror("pthread_create");
        abort();
    }

    pthread_join(th2,&th2ret);

    printf("out thread:%d\n",gettid());
    return NULL;
}


int main(int argc, char** argv)
{
    int fd=0;
    int efd;
    int ret;
    int i;
    struct epoll_event event;
    struct epoll_event* events;
    char buf[READMAX];

    pthread_t th;
    void* thret;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_barrier_init(&barrier,NULL,1+1);  //1+1个等待

    printf("in main, pid:%d\n",getpid());

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


    ret = pthread_create(&th,NULL,threadfn,NULL);
    if (ret){
        perror("pthread_create");
        abort();
    }

    events = calloc(MAXEVENTS, sizeof(struct epoll_event));
    printf("epoll_wait stdin, press enter to continue...\n");
    pthread_barrier_wait(&barrier);     //起跑枪“砰!”

    ret = epoll_wait (efd, events, MAXEVENTS, -1);
    printf("epoll_wait ret:%d, errno:%d\n",ret,errno);
    for(i=0; i<ret; i++){
        printf("evt flg for events %d is %x\n",i,events[i].events);
    }
    ret = read(events[0].data.fd,buf,READMAX);
    printf("read up %d bytes,wait thread\n",ret);

    pthread_join(th,&thret);

    return 0;
}
