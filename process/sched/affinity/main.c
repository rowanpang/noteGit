#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/sysinfo.h>
#include<time.h>

#define __USE_GNU
#include<sched.h>
#include<ctype.h>
#include<string.h>

#define COUNT 300000
#define MILLION 1000000L
#define NANOSECOND 1000

void test_func()
{
    int i = 0;
    unsigned long long result = 0;;

    for(i = 0; i<8000 ;i++) {
	result += 2;
    }

}

void print_cpu_set(cpu_set_t cpu_set)
{
    unsigned char flag = 0;
    printf("Cpu affinity is:");
    for (unsigned int i = 0; i < sizeof(cpu_set_t); i ++) {
        if (CPU_ISSET(i, &cpu_set)) {
            if (flag == 0) {
                flag = 1;
                printf("%d", i);
            } else {
                printf(",%d", i);
            }
        }
    }
    printf(".\n");
}

int main(int argc,char* argv[])
{
    int i;
    struct timespec sleeptm;
    long interval;
    struct timeval tend,tstart;
    struct tm lcltime = {0};
    struct sched_param param;
    int sched_method_cur;
    int ret = 0;

    cpu_set_t tgtset ;
    CPU_ZERO(&tgtset);

    if (sched_getaffinity(0, sizeof(cpu_set_t), &tgtset) == -1) {
        perror("get cpu affinity failed.\n");
	return -1;
    } else {
	print_cpu_set(tgtset);
    }

    CPU_ZERO(&tgtset);
    CPU_SET(3,&tgtset);                     //move to cpu3
    if (sched_setaffinity(0, sizeof(cpu_set_t), &tgtset) == -1) {
	printf("warning: could not set CPU affinity, continuing...\n");
    }

    printf("going sleep 1 PID(%ld)\n",getpid());
    sleep(1);

    if (sched_getaffinity(0, sizeof(cpu_set_t), &tgtset) == -1) {
        perror("get cpu affinity failed.\n");
	return -1;
    } else {
	print_cpu_set(tgtset);
    }

    return 0;
}
