#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#define gettid() syscall(__NR_gettid)

void* threadfn(void *arg)
{
    printf("in thread:%d,parent:%d \n",gettid(),getppid());
    while(1){
        ;
    }

    return NULL;
}

int main(int argc,char** argv)
{
    int ret;
    int *status = &ret;
    int i,nth = 4;
    pthread_t *th;
    printf("in main:%d,parent:%d,will create %d threads \n",getpid(),getppid(),nth);

    th = malloc(nth*sizeof(pthread_t));

    for(i=0;i<nth;i++){
        ret = pthread_create(th++,NULL,threadfn,NULL);
        if(ret){
            perror("error when pthread create");
            goto ERROR_1;
        }
    }

    pause();
    printf("thread finished ret:%d\n",ret);
    ret = 0;
ERROR_1:
    return ret;
}
