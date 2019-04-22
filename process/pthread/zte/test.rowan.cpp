#include "pthread.h"
#include "sys/time.h"
#include "errno.h"
#include "string.h"
#include "unistd.h"
#include "iostream"

typedef unsigned int        UINT32;
typedef unsigned char BOOLEAN;
typedef void*               (HANDLE_FUNC)(void*);
typedef void*               LPVOID;

using namespace std;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//pthread_mutex_init(mutex, NULL);
//
//
//

BOOLEAN
CREATE_THREAD(
	    HANDLE_FUNC startaddr,    /* thread function addr */
	    LPVOID parg,      /* function arg */
	    UINT32 stack,         /* function stack size */
	    pthread_t* dwThread, /* return thread id */
	    UINT32* phandle)  /* return the handle of thread*/
{
    int hthread;

    pthread_t   dwThreadId;
    pthread_attr_t      thread_attr;

    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&thread_attr, PTHREAD_SCOPE_SYSTEM);
    if (stack > 0){
        pthread_attr_setstacksize(&thread_attr, stack);
    }

    if (0 != pthread_create(&dwThreadId, &thread_attr, (void *(*)(void*))startaddr, (LPVOID)parg)) {
	hthread  = 0;
    } else {
	*dwThread = dwThreadId;
	hthread  = 1;
    }
    pthread_attr_destroy(&thread_attr);

    if (hthread == 0)
        return 0;
    else
        return 1;
}

void cleanup(void* arg){
    int i = *(int*)arg;
    cout<<"thread "<<i<<" call cleanup"<<endl;
    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&cond);
}

void *thread0(void* arg){
    int i = pthread_self(), ret = 0;
    pthread_cleanup_push(&cleanup, &i);
    int loop = 0;
    cout<<"thread0 start"<<endl;
    while (ret = pthread_mutex_trylock(&mutex)){
            loop++;
            cout<<"thread0 timespawn "<<loop*3<< " "<<strerror(ret)<<endl;
            sleep(3);
        }
    cout<<"thread0 end"<<endl;
    pthread_mutex_unlock(&mutex);
    pthread_cleanup_pop(1);
    cout<<"---thread0 return"<<endl;
    return NULL;
}

void *thread(void* arg){
    int i = pthread_self();
    pthread_cleanup_push(&cleanup, &i);
    pthread_mutex_lock(&mutex);
    cout<<"thread start"<<endl;
    sleep(3);
    cout<<"thread end"<<endl;
    cout<<"thread unlock "<<pthread_mutex_unlock(&mutex)<<endl;
    cout<<"thread broad "<<pthread_cond_broadcast(&cond)<<endl;
    pthread_cleanup_pop(1);
    cout<<"---thread return"<<endl;
    return NULL;
}

int main(){
    pthread_t tid[2];
    UINT32     dwThreadId=0;
    cout<<"main create"<<endl;
    CREATE_THREAD(thread, NULL, 1024 * 1024,tid,(UINT32*) &dwThreadId);
    sleep(1);
    CREATE_THREAD(thread0, NULL, 1024 * 1024,&tid[1],(UINT32*) &dwThreadId);

    cout<<"--main end"<<endl;
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    cout<<"main exit"<<endl;
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    sleep(10);
    return 0;
}
