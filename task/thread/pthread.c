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
	pthread_mutex_lock(&i_mutex);
	global++;
	printf("in thread:%d,val:%d\n",gettid(),global);
	pthread_mutex_unlock(&i_mutex);
	while(1){
		sleep(10);
	}
	
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
	global++;
	printf("in main:%d\n",getpid());
	pthread_t thexec;

	int ret = 0;
	//early created not schedul 
	for(int i=0;i<num;i++){
		pthread_mutex_unlock(&i_mutex);
		ret = pthread_create(th+i,NULL,threadfn,&i);
		pthread_mutex_lock(&i_mutex);
	}
	pthread_mutex_unlock(&i_mutex);
	
	sleep(3);
	cpid = fork();
	if(cpid == 0){
		/*child*/
		sleep(3);
		exit(0);
	}else{
		printf("child %d\n",cpid);
		sleep(3);
	}


	pthread_create(&thexec,NULL,threadfnexec,NULL);
	
	for(int i = 0;i<num;i++){
		/*
		 *pthread_cancel(*(th+i));
		 */
		pthread_join(thexec,NULL);
	}

	return 0;
}
