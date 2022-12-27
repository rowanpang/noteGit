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

void* threadfn(void *arg)
{
    int ret;
    printf("in thread:%d\n",gettid());
    while(1){
	ret = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if(ret){
	    perror("when disable cancel");
	    goto FINISH;
	}
	printf("start with disable canceled\n");
        sleep(5);
	printf("start enable canceled\n");
	ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if(ret){
	    perror("when enable cancel");
	    goto FINISH;
	}
        sleep(10);	//give a chance to cancel
    }

    ret = 1;
FINISH:
    /*return (void *)(long)ret;*/
    return ERR_PTR(ret);
}

int main(int argc,char** argv)
{
    int ret;
    void *status;
    pthread_t th;
    printf("in main:%d,canceled:%p\n",getpid(),PTHREAD_CANCELED);

    ret = pthread_create(&th,NULL,threadfn,NULL);
    if(ret){
	perror("error when pthread create");
	goto ERROR_1;
    }

    ret = pthread_cancel(th);
    if(ret){
	perror("error when pthread create");
	goto ERROR_1;
    }
    printf("sig cancele was emitted\n");

    sleep(1);
    pthread_join(th,&status);

    if(status == PTHREAD_CANCELED){
	printf("thread was canceled\n");
    }else{
	printf("thread exit status %d\n",PTR_ERR(status));
    }

    ret = 0;
ERROR_1:
    return ret;
}
