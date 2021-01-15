#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef struct
{
    int tc_number;
    char ap_name[5];
    char mymessage[20];
} COMM_TABLE;

int main(int argc,char **argv)
{
    /* local variables */
    int ret= 0;
    key_t key;
    int i;
    int shm_id;
    int found = 0;
    COMM_TABLE *comm_reg;

    key = ftok(".",'w');

    /* create a share memory if not exist */
    if ((shm_id = shmget(key ,sizeof(COMM_TABLE),IPC_CREAT|IPC_EXCL|0666)) == -1) {
    /* share memory has been created */
        if ((shm_id = shmget(key , sizeof(COMM_TABLE),0)) == -1)
        {
            printf("error = %d\n", errno);
            return ret;
        }
    }

    comm_reg = (COMM_TABLE *) shmat(shm_id, NULL, 0);
    comm_reg->tc_number= 10000000;

    return 0;
}
