#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "errptr.h"

#define gettid() syscall(__NR_gettid)

void sleeptest()
{
    sleep(1);
}

void* threadfn(void *arg)
{
    unsigned long idx;
    int urand;

    idx = (unsigned long) arg;
    urand = 1000*(rand()%900 + 100);
    printf("idx:%03ld,tid:%d,parent:%d,urand:%d \n",idx,gettid(),getppid(),urand);

    usleep(urand);              //random sleep [100,1000)ms
    while(1) {
        sleeptest();
    }

    return ERR_PTR(0);
}

#define THREADS 100

int main(int argc,char** argv)
{
    int ret;
    pthread_t threads[THREADS];
    unsigned long i, *status[THREADS];

    printf("in main:%d,parent:%d \n",getpid(),getppid());

    /* Spawn all testing threads */
    for (i = 0; i < THREADS; i++) {
        if (pthread_create(&threads[i], NULL, threadfn, (void *)i)) {
            perror("pthread_create() failed");
            ret = -1;
            goto fail;
        }
    }

    sleep(20);

    for (i = 0; i < THREADS; i++) {
        pthread_cancel(threads[i]);
        printf("in main join thidx:%03ld\n",i);

        if (pthread_join(threads[i], (void **)&(status[i]))) {
            perror("pthread_join() failed");
            ret = -1;
            goto fail;
        }

        if (status[i] == PTHREAD_CANCELED){
            //printf("thread %03ld caceled\n",i);
            continue;
        } else if (IS_ERR(status[i])) {
            printf("%ldth thread failed to join with %ld status\n", i, PTR_ERR(status[i]));
            ret = -1;
            goto fail;
        }
    }

    ret = 0;

fail:
    return ret;
}
