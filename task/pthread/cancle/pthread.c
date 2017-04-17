#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#define gettid() syscall(__NR_gettid)

void* threadfn(void *arg)
{
    printf("in thread:%d\n",gettid());
    while(1){
        sleep(10);
    }

    return NULL;
}

int main(int argc,char** argv)
{
    int ret;
    int *status = &ret;
    pthread_t th;
    printf("in main:%d\n",getpid());

    ret = pthread_create(&th,NULL,threadfn,NULL);
    if(ret){
	perror("error when pthread create");
	goto ERROR_1;
    }
    sleep(3);

    pthread_cancel(th);
    pthread_join(th,(void **)&status);

    printf("thread finished ret:%d\n",ret);
    ret = 0;
ERROR_1:
    return ret;
}
