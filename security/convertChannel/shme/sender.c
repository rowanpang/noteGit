#define _GNU_SOURCE
#include "comm.h"
#include <sched.h>

void sendMsgOne();
void sendMsgZero();

int main()
{
    char ch;
    FILE *fp;
    int cnt=0;
    cpu_set_t cpuMask;

    CPU_ZERO(&cpuMask);
    CPU_SET(0,&cpuMask);
    /*sched_setaffinity(0,sizeof(cpuMask),&cpuMask);*/

    if ((fp = fopen("./sender_text.txt", "r")) == NULL)
    {
        printf("fopen error.\n");
        return -1;
    }

/*
 *    // 申请一块共享内存作为同步信号传输
 *    int shmid_syn = create_shm_syn(MEM_SIZE);
 *    if (shmid_syn < 0)
 *    {
 *        printf("Error: create shm for syn error!\n");
 *        return -1;
 *    }
 *
 *    printf("------------------------------------\n");
 *    printf("\t!!Begin to send message!!\n");
 *    printf("------------------------------------\n");
 *
 */
    synchronize(1);
    // 传输数据
    while ((ch = fgetc(fp)) != EOF)
    {
	printf("Current char is: %c,cnt %d\n", ch,cnt);
        if (ch == '0')
            sendMsgZero();
        else if (ch == '1')
            sendMsgOne();
        else
        {
            ;
        }
	cnt++;
    }

    // 关闭同步信号
    /*destroy_shm(shmid_syn);*/


    fclose(fp); // Close the opened file

    printf("------------------------------------\n");
    printf("\t!!Send message over!!\n");
    printf("------------------------------------\n");

    return 0;
}

void sendMsgZero()
{
    usleep(TIME);
    //sleep(1);
    /*
     *printf("Send message: 0 \n");
     *printf("------------------------------------\n");
     */
}

void sendMsgOne()
{
    int shmid = create_shm(MEM_SIZE);
    //sleep(1);
    usleep(TIME);
    if(shmid > 0)
    {
        destroy_shm(shmid);
	/*
	 *printf("Send message: 1, shmid is: %d\n", shmid);
	 *printf("------------------------------------\n");
	 */
    }
    else
    {
        printf("Error: send message one error!!\n");
    }
}
