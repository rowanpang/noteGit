#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#define gettid() syscall(__NR_gettid)

pthread_once_t oncectl = PTHREAD_ONCE_INIT;
void once_init(void)
{
    printf("in func once_init,tid:%d\n",gettid());
}

void* threadfn(void *arg)
{
    printf("tid:%d,parent:%d \n",gettid(),getppid());
    pthread_once(&oncectl,once_init);
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
    printf("in main,tid:%d,pid:%d,parent:%d \n",gettid(),getpid(),getppid());
    if (argc==1) {
        printf("once func will run by main thread\n");
        pthread_once(&oncectl,once_init);
    } else {
        printf("once func will run by non main thread\n");
    }

    ret = pthread_create(&th,NULL,threadfn,NULL);
    if(ret){
	perror("error when pthread create");
	goto ERROR_1;
    }
    /*exit(0);*/
    sleep(5);

    if (argc!=1) {
        pthread_once(&oncectl,once_init);
    }
    printf("main exit,ret:%d\n",ret);
    ret = 0;
ERROR_1:
    return ret;
}
