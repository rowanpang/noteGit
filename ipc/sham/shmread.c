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

    /* share memory has been created */
    if ((shm_id = shmget(key , sizeof(COMM_TABLE),0)) == -1){
        printf("error = %d\n", errno);
        return ret;
    }
    comm_reg = (COMM_TABLE *) shmat(shm_id, NULL, 0);
    printf("tc number=%d!!!\n", comm_reg->tc_number);

    /* kill share memory */
    shmctl(shm_id,IPC_RMID,0);
    exit(0);
}
