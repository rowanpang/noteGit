#include "comm.h"

//
static int comm_shm(int size, int shmflag)
{
    key_t key = ftok(PATHNAME, PROJ_ID); // 获取key
    if(key < 0){
        perror("ftok");
        return -1;
    }
    int shmid = shmget(key,  size, shmflag);
    if(shmid < 0){
        perror("shmget");
        return -2;
    }

    return shmid;
}

static int comm_shm1(int size, int shmflag)
{
    key_t key = ftok(PATHNAME, PROJ_ID_1); // 获取key
    if(key < 0){
        perror("ftok");
        return -1;
    }
    int shmid = shmget(key,  size, shmflag);
    if(shmid < 0){
        perror("shmget");
        return -2;
    }

    return shmid;
}

int create_shm (int size)
{
    return comm_shm(size, IPC_CREAT|IPC_EXCL|0666);
}

int create_shm_syn (int size)
{
    return comm_shm1(size, IPC_CREAT|IPC_EXCL|0666);
}

int get_shmid()
{
    return comm_shm(0, IPC_CREAT);
}

int destroy_shm(int shmid)
{
    if( shmctl( shmid, IPC_RMID, NULL) < 0)
    {
	perror("shmctl");
	return -1;
    }
    return 0;
}

void synchronize(int sender)
{
    time_t start = time(NULL);
    printf("---------------Synchronizing start, %ld ...\n",start);
    if(sender){
    }

    while(1)
    {
	if(start % 10 == 0){
	    break;
	}

	usleep(1000*1);
	start = time(NULL);
    }

    printf("--------------Synchronizing ok, %ld ...\n",start);
}
