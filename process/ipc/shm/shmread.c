#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef struct {
    int tc_number;
    char ap_name[5];
    char mymessage[20];
} COMM_TABLE;

int main(int argc,char **argv)
{
    int ret= 0;
    key_t key;
    int i;
    int shm_id;
    int found = 0;
    COMM_TABLE *comm_reg;
    char * pointer;
    key = ftok(".",'w');

    if ((shm_id = shmget(key ,sizeof(COMM_TABLE),IPC_CREAT|IPC_EXCL|0666)) == -1) {
        perror("share memory is already exit!!");
        if ((shm_id = shmget(key , sizeof(COMM_TABLE),0)) == -1) {
            perror("error when shmget an already exit shm");
            return -1;
        }
    }

    comm_reg = (COMM_TABLE *) shmat(shm_id, NULL, 0);
    printf("tc number=%d!!!\n", comm_reg->tc_number);

    shmdt(comm_reg);
    /* kill share memory */
    shmctl(shm_id,IPC_RMID,0);
    exit(0);
}
