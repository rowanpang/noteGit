#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#define gettid() syscall(__NR_gettid)

#define RINGBUFSIZE 32
#define RINGBUFMASK (RINGBUFSIZE-1)
struct ringbuf {
    int buf[RINGBUFSIZE];
    unsigned char prod;
    unsigned char cons;
    unsigned char notify_on_cons;
    pthread_mutex_t lock;
    pthread_cond_t event;
};


int msprod, mscons;

void* tfnprod(void *arg)
{
    struct ringbuf *rb = arg;
    printf("in thread prod:%d\n",gettid());
    while(1){
        pthread_mutex_lock(&rb->lock);
        if(rb->prod - rb->cons == RINGBUFSIZE){         //但前的prod生产了,但cons还没有消费
            rb->notify_on_cons = rb->cons + 5;
            printf("ringbuf full,well prod:%d, wait on cons:%d\n",rb->prod,rb->notify_on_cons);
            pthread_cond_wait(&rb->event,&rb->lock);
            printf("ringbuf full,wait idx:%d ok\n",rb->prod);
        }

        rb->buf[rb->prod & RINGBUFMASK] = rb->prod;
        printf("prod idx:%u,tmp:%d\n",rb->prod,rb->prod);
        rb->prod++;

        pthread_mutex_unlock(&rb->lock);
        usleep(1000*msprod);
    }

    return NULL;
}

void* tfncons(void *arg)
{
    struct ringbuf *rb = arg;
    int tmp;
    printf("in thread cons:%d\n",gettid());
    while(1){
        pthread_mutex_lock(&rb->lock);
        if(rb->prod == rb->cons){
            printf("ringbuf empty,sleep\n");
            goto sleep;
        }

        tmp = rb->buf[rb->cons & RINGBUFMASK];
        printf("consed idx:%u,tmp:%d\n",rb->cons,tmp);
        rb->cons++;

        if ( rb->cons == rb->notify_on_cons){
            printf("cons well idx:%u,signal proder\n",rb->cons);
            pthread_cond_signal(&rb->event);
        }

    sleep:
        pthread_mutex_unlock(&rb->lock);
        usleep(1000*mscons);
    }

    return NULL;
}

int main(int argc,char** argv)
{
    int ret;
    int *status = &ret;
    struct ringbuf rb;
    pthread_t thprod,thcons;
    msprod = 10;
    mscons = 10;

    if(argc == 3){
        msprod = atoi(argv[1]);
        mscons = atoi(argv[2]);
    }

    printf("in main:%d,msprod:%d,mscons:%d\n",getpid(),msprod,mscons);
    sleep(3);

    rb.prod = 0;
    rb.cons = 0;
    rb.notify_on_cons = 0;
    pthread_mutex_init(&rb.lock,NULL);
    pthread_cond_init(&rb.event,NULL);

    ret = pthread_create(&thprod,NULL,tfnprod,&rb);
    if(ret){
	perror("error when pthread create");
	goto ERROR_1;
    }

    ret = pthread_create(&thcons,NULL,tfncons,&rb);
    if(ret){
	perror("error when pthread create");
	goto ERROR_1;
    }

    ret = pthread_join(thprod,NULL);
    printf("thread prod finish ret:%d\n",ret);

    ret = pthread_join(thcons,NULL);
    printf("thread cons finish ret:%d\n",ret);

    ret = 0;
ERROR_1:
    return ret;
}
