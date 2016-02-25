#include <stdio.h>
#include <pthread.h>

int global = 0;
pthread_mutex_t i_mutex = PTHREAD_MUTEX_INITIALIZER;

void* threadfn(void *arg)
{
	pthread_mutex_lock(&i_mutex);
	global++;
	printf("in thread:%d,val:%d\n",*(int *)arg,global);
	pthread_mutex_unlock(&i_mutex);
	return NULL;
}

int main(int argc,char** argv)
{
	int num = 30;
	pthread_t th[num];
	global++;
	printf("in main:%d\n",global);

	while(1){
		printf("in main:%d\n",global);
	}

	int ret = 0;
	//early created not schedul 
	for(int i=0;i<num;i++){
		pthread_mutex_unlock(&i_mutex);
		ret = pthread_create(th+i,NULL,threadfn,&i);
		pthread_mutex_lock(&i_mutex);
	}
	pthread_mutex_unlock(&i_mutex);

	for(int i = 0;i<num;i++){
		pthread_join(*(th+i),NULL);
	}

	return 0;
}
