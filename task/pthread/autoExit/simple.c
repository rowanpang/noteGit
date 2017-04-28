#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#include <rowan/errptr.h>

#define gettid() syscall(__NR_gettid)

static int threadExit=0;

void* threadfn(void *arg)
{
    int ret;
    printf("in thread:%d\n",gettid());
    while(1){
	sleep(2);
	
	if(threadExit){
	    break;
	}
    }

    ret=0;

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
    sleep(5);
    threadExit=1;

    pthread_join(th,&status);

    printf("thread finished ret:%d\n",PTR_ERR(status));
    ret = 0;
ERROR_1:
    return ret;
}
