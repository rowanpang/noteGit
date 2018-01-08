#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#define gettid() syscall(__NR_gettid)

int global = 0;
pthread_mutex_t i_mutex = PTHREAD_MUTEX_INITIALIZER;

void* threadfn(void *arg)
{
    int i;
    sleep(1);
    for (i = 0; i< 100000; i++){
	pthread_mutex_lock(&i_mutex);
	global++;
	pthread_mutex_unlock(&i_mutex);
    }
    printf("in thread:%d,val:%d\n",gettid(),global);
    sleep(1);
    
    return NULL;
}

void* threadfnexec(void *arg)
{
    printf("in thread:%d\n",gettid());
    execve("/bin/bash",NULL,NULL);
    return NULL;    
}
int main(int argc,char** argv)
{
    int num = 3;
    int cpid;
    pthread_t th[num];
    printf("in main:%d\n",getpid());
    pthread_t thexec;

    int ret = 0;

    for(int i=0;i<num;i++){
        ret = pthread_create(th+i,NULL,threadfn,&i);
    }
    
    for(int i = 0;i<num;i++){
	pthread_join(*(th+i),NULL);
    }

    return 0;
}
