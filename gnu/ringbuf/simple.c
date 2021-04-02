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
    pthread_mutex_t lock;
};

void* tfnprod(void *arg)
{
    struct ringbuf *rb = arg;
    printf("in thread prod:%d\n",gettid());
    while(1){
        pthread_mutex_lock(&rb->lock);
        if(rb->prod - rb->cons == RINGBUFSIZE){
            printf("ringbuf full,sleep\n");
            goto sleep;
        }

        rb->buf[rb->prod & RINGBUFMASK] = rb->prod;
        printf("prod idx:%u,tmp:%d\n",rb->prod,rb->prod);
        rb->prod++;

    sleep:
        pthread_mutex_unlock(&rb->lock);
        usleep(1000*5);
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
        printf("cons idx:%u,tmp:%d\n",rb->cons,tmp);
        rb->cons++;
    sleep:
        pthread_mutex_unlock(&rb->lock);
        usleep(1000*10);
    }

    return NULL;
}

int main(int argc,char** argv)
{
    int ret;
    int *status = &ret;
    struct ringbuf rb;
    pthread_t thprod,thcons;

    printf("in main:%d\n",getpid());

    rb.prod = rb.cons = 0;
    pthread_mutex_init(&rb.lock,NULL);

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
