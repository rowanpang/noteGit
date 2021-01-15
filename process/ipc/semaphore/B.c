#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

#define PERMS S_IRUSR|S_IWUSR
#define MAX_CANON 100

union semun {
    int val;
    struct semid_ds *buf;
};

int del_semaphore(int semid)
{
    int ret;
    ret = semctl(semid,0,IPC_RMID);
    if (ret == -1){
        fprintf(stderr,"[%d]:Destroy Semaphore Error:%s\n\a",
            getpid(),strerror(errno));
    }

    return ret;
}

int semInit(int semid)
{
    int ret;
    union semun tmp;
    struct semid_ds dsInfo;

    tmp.buf = &dsInfo;
    ret = semctl(semid,0,IPC_STAT,tmp);
    if(tmp.buf->sem_otime){
        /*
         *semop 操作之后就不能再setval,这样会影响locking操作.
         *semop 操作之前,semval任何进程都可以初始化
         */
        printf("has inited,semop operated\n");
        return 0;
    }else{
        printf("will init sem %d\n",semid);
    }

    tmp.val = 1;
    ret = semctl(semid,0,SETVAL,tmp);
    if (ret == -1){
        fprintf(stderr,"[%d]:Init Semaphore Error:%s\n\a",
            getpid(),strerror(errno));
        exit(-1);
    }
}

int semP(int semid,int snum)
{
    struct sembuf stmp;
    stmp.sem_num = snum;
    stmp.sem_op = -1;
    stmp.sem_flg = SEM_UNDO;

    /*locking*/
    if(semop(semid,&stmp,1)==-1) {
        fprintf(stderr,"[%d]:semP Decrement Semaphore Error:%s\n\a",
            getpid(),strerror(errno));
        del_semaphore(semid);

        exit(1);
    }

    printf("semP errno:%d\n",errno);
    return 0;
}

int semV(int semid,int snum)
{
    struct sembuf stmp;
    stmp.sem_num = snum;
    stmp.sem_op = 1;
    stmp.sem_flg = SEM_UNDO;

    /*unlocking*/
    if(semop(semid,&stmp,1)==-1) {
        fprintf(stderr,"[%d]:semV Increment Semaphore Error:%s\n\a",
            getpid(),strerror(errno));
        del_semaphore(semid);

        exit(1);
    }
}

int main(int argc,char **argv)
{
    char buffer[MAX_CANON],*c;
    int i,n;
    int semid,semop_ret,status;
    pid_t cpid;

    if((semid=semget((key_t)123,1,IPC_CREAT|PERMS))==-1) {
        fprintf(stderr,"[%d]:Acess Semaphore Error:%s\n\a",
                getpid(),strerror(errno));
        exit(1);
    }else{
        fprintf(stderr,"semid:%d\n",semid);
    }
    semInit(semid);

    printf("press Enter try to locking critical area\n");
    getchar();

    printf("try locking....,semvar:%d\n",semctl(semid,0,GETVAL));
    semP(semid,0);
        printf("locked!enter critical area,val:%d\n",
                    semctl(semid,0,GETVAL));
        printf("press Enter to unlock \n");
        getchar();
        printf("unlocking.....\n");
    semV(semid,0);

    printf("try locking2....,semvar:%d\n",semctl(semid,0,GETVAL));
    semP(semid,0);
        printf("locked!enter critical area,val:%d\n",
                    semctl(semid,0,GETVAL));
        printf("press Enter to unlock \n");
        getchar();
        printf("unlocking.....\n");
    semV(semid,0);

    printf("out critical area\n");
    /*
     *semV之后才rm sem的,所以等待进程可以正常唤醒,而不会报EIDRM.
     */
    del_semaphore(semid);

    return 0;
}
