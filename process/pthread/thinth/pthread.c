#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#define gettid() syscall(__NR_gettid)

static inline void * ERR_PTR(long error)
{
    return (void *) error;
}

static inline long PTR_ERR(const void* ptr)
{
    return (long)ptr;
}

void* threadfn2(void *arg)
{
    int ret;
    int i = 0;

    while(i++ < 5){
        printf("hello world: %d\n",i);
        sleep(1);
    }

    ret = 0;
    return ERR_PTR(ret);
}

void* threadfn(void *arg)
{
    int ret;
    pthread_t th2;
    void *status;
    printf("in thread:%d\n",gettid());

    ret = pthread_create(&th2,NULL,threadfn2,NULL);
    if(ret){
	perror("error when pthread create");
	goto ERROR_1;
    }

    sleep(2);
    printf("th join th2\n");
    pthread_join(th2,&status);

    if(status == PTHREAD_CANCELED){
	printf("th2 was canceled\n");
    }else{
	printf("th2 exit status %d\n",PTR_ERR(status));
    }

ERROR_1:
    ret = -1;

FINISH:
    ret = 0;
    /*return (void *)(long)ret;*/
    return ERR_PTR(ret);
}

int main(int argc,char** argv)
{
    int ret;
    void *status;
    pthread_t th;
    printf("in main:%d\n",getpid());

    ret = pthread_create(&th,NULL,threadfn,NULL);
    if(ret){
	perror("error when pthread create");
	goto ERROR_1;
    }

    sleep(1);
    pthread_join(th,&status);

    if(status == PTHREAD_CANCELED){
	printf("th was canceled\n");
    }else{
	printf("th exit status %d\n",PTR_ERR(status));
    }

    ret = 0;
ERROR_1:
    return ret;
}
