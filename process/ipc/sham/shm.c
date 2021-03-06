#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMKEY 74
#define K 1024
int shmid;

void cleanup(int i)
{
    printf("cache signal,do cleanup\n");
    shmctl(shmid,IPC_RMID,0);
    exit(0);
}

int main(int argc,char **argv)
{
    int i;
    int *pint;
    char *addr1,*addr2;
    for (i=0;i<20;i++){
        signal(i,cleanup);
    }

    shmid=shmget(SHMKEY,128*K,0777|IPC_CREAT);

    addr1=shmat(shmid,0,0);
    addr2=shmat(shmid,0,0);

    printf("addr1 0x%x addr2 0x%x\n",addr1,addr2);

    pint=(int*)addr1;
    for (i=0;i<256;i++){
        *(pint++)=i;
    }
    pint=(int*)addr1;
    *pint=256;

    pint=(int*)addr2;
    for (i=0;i<256;i++){
        printf("index:%3d\tvalue:%d\n",i,*(pint++));
    }

    shmdt(addr1);
    shmdt(addr2);
    printf("press Enter to quit:\n");
    getchar();
    cleanup(0);
}
