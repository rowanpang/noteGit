#define _GNU_SOURCE
#include "comm.h"
#include <sched.h>

int main()
{
    int syn_flag = 0;
    int cnt=0;
    cpu_set_t cpuMask;

    CPU_ZERO(&cpuMask);
    CPU_SET(0,&cpuMask);
    /*sched_setaffinity(0,sizeof(cpuMask),&cpuMask);*/

    // 监听同步信号
    /*
     *while(1)
     *{
     *    int shmid_syn = create_shm_syn(MEM_SIZE);
     *    if (shmid_syn < 0) {
     *        syn_flag = 1;
     *        break;
     *    }
     *    else
     *        destroy_shm(shmid_syn);
     *}
     *printf("-------------------------\n");
     *printf("Begin to receive message!!\n");
     *printf("-------------------------\n");
     */

    synchronize(0);

    int shmid = 0;
    while(1)
    {
        int shmid = create_shm(MEM_SIZE);
        if (shmid > 0)
        {
            printf("Receive message: 0,cnt:%d\n",cnt);
            printf("-------------------------\n");
            destroy_shm(shmid); // Release some share memory!
        }
        else
        {
            printf("Receive message: 1,cnt:%d\n",cnt);
            printf("-------------------------\n");
        }
	cnt++;

        usleep(TIME);

	if (cnt > 40){
	    break;
	}
	/*
         * 监测结束同步信号
         *int shmid_syn = create_shm_syn(MEM_SIZE);
         *if (shmid_syn > 0)
         *{
         *    destroy_shm(shmid_syn);
         *    break;
         *}
	 */
    }

    printf("-------------------------\n");
    printf("Receieve message over!!\n");
    printf("-------------------------\n");

    return 0;
}
