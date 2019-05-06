//#pragma pack(1)

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

struct thread_data_t{
    pthread_mutex_t mutex;
    int a;
    char b;
};


int main()
{

    struct thread_data_t *p = (struct thread_data_t *) malloc(2* sizeof(struct thread_data_t));
    pthread_mutex_init(&p[1].mutex,NULL);

    printf("sizeof: %d\n",sizeof(struct thread_data_t));

    pthread_mutex_lock(&p[1].mutex);

    return 0;
}
