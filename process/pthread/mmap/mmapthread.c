#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int global;
pthread_mutex_t i_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

typedef struct thread_args {
    int myid;
    int iterations;
    char *fpath;
    unsigned long uelapsed;
} thread_args_t;

#define MAX_ERRNO    4095
#define IS_ERR_VALUE(x) ((unsigned long)(void *)(x) >= (unsigned long)-MAX_ERRNO)

static inline void * ERR_PTR(long error)
{
    return (void *) error;
}

static inline long PTR_ERR(const void *ptr)
{
    return (long) ptr;
}

static inline int IS_ERR_OR_NULL(const void *ptr)
{
    return (!ptr) || IS_ERR_VALUE((unsigned long)ptr);
}

static inline int IS_ERR(const void *ptr)
{
    return IS_ERR_VALUE((unsigned long)ptr);
}

void* threadfn(void *tharg)
{
    thread_args_t *arg = (thread_args_t*)tharg;
    int iters = arg->iterations;
    int id = arg->myid;
    char *fpath = arg->fpath;
    int fd;
    char *maddr;
    int i;
    int rc;
    struct timeval ts, te;
    unsigned long elapsed;

    printf("thread: %d, iters: %d\n",id,iters);


    pthread_barrier_wait(&barrier);

    gettimeofday(&ts,NULL);
    for (i = 0; i < iters; i++){
	fd = open(fpath,O_RDWR|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
	if (fd < 0) {
	    printf("open file name %s,errno:%d \n", fpath,errno);
	    return ERR_PTR(-errno);
	}
	maddr = mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

	if ( maddr == MAP_FAILED){
	    printf("mmap() failed, errno %d\n", errno);
	    rc = -errno;
	    goto out;
	}

	rc = munmap(maddr, 4096);
	if (rc < 0) {
	    printf("munmap() failed, errno %d\n", errno);
	    goto out;
	}
	close(fd);
    }
    gettimeofday(&te,NULL);

    elapsed = (te.tv_sec - ts.tv_sec)*1.0e6 + (te.tv_usec-ts.tv_usec);
    arg->uelapsed = elapsed;
    rc = 0;
    return ERR_PTR(rc);

out:
    close(fd);
    return ERR_PTR(rc);
}

int main(int argc,char** argv)
{
    int rc = 0;
    int thnum = 3;
    int iters=0;
    int cpid;
    pthread_t *thgroup;
    thread_args_t *thargs;

    void * thret;

    if (argc !=3 ){
	printf("usage: %s <threadnum> <threaditers>\n", argv[0]);
	exit(-1);
    }
    thnum = atoi(argv[1]);
    iters = atoi(argv[2]);
    thgroup = malloc(sizeof(pthread_t)*thnum);
    thargs = malloc(sizeof(thread_args_t)*thnum);
    pthread_barrier_init(&barrier,NULL,thnum);


    printf("in main:%d\n",getpid());


    for(int i=0;i<thnum;i++){
	thargs[i].myid = i;
	thargs[i].iterations = iters;
	thargs[i].fpath = "/run/rmmap.test";

        rc = pthread_create(thgroup+i,NULL,threadfn,thargs+i);
	if (rc != 0) {
	    printf("error: pthread_create %d\n", rc);
	    return rc;
	}
    }


    printf("pthread joining \n");
    for(int i = 0;i<thnum;i++){
	rc = pthread_join(*(thgroup+i),&thret);
	printf("pthread %d,finished", i);
	if (rc != 0) {
	    printf("error: pthread %d cannot be joined", i);
	    return rc;
	}

	if (IS_ERR(thret)){
	    printf(" errno:%ld, str:%s\n",PTR_ERR(thret),strerror(-PTR_ERR(thret)));
	}else{
	    printf(",elapsed usec :%ld\n",thargs[i].uelapsed);
	}
    }

    free(thgroup);
    free(thargs);

    return 0;
}
