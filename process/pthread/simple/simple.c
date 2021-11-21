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
    return NULL;
    exit(0);			//will also exit the whole thread group
    while(1){
	sleep(100);
    }

    return NULL;
}

int main(int argc,char** argv)
{
    int ret;
    int *status = &ret;
    pthread_t th;
    printf("in main:%d,parent:%d \n",getpid(),getppid());

    ret = pthread_create(&th,NULL,threadfn,NULL);
    if(ret){
	perror("error when pthread create");
	goto ERROR_1;
    }
    /*exit(0);*/
    sleep(5);

    printf("thread finished ret:%d\n",ret);
    ret = 0;
ERROR_1:
    return ret;
}
