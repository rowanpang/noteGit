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

void print_cpu_mask(cpu_set_t cpu_mask)
{
    unsigned char flag = 0;
    printf("Cpu affinity is:");
    for (unsigned int i = 0; i < sizeof(cpu_set_t); i ++) {
        if (CPU_ISSET(i, &cpu_mask)) {
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

    if(argc != 3) {
	fprintf(stderr,"usage: %s sched_method sched_priority\n",argv[0]);
	return -1;
    }

    int sched_method = atoi(argv[1]);
    int sched_priority = atoi(argv[2]);
    if(sched_method > 2 || sched_method < 0) {
	fprintf(stderr,"sched_method scope [0,2]\n");
	return -2;
    }

    if(sched_priority > 99 || sched_priority < 1) {
	fprintf(stderr,"sched_priority scope [1,99]\n");
	return -3;
    }

    cpu_set_t mask ;
    CPU_ZERO(&mask);

    if (sched_getaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
        perror("get cpu affinity failed.\n");
	return -1;
    } else {
	print_cpu_mask(mask);
    }

    CPU_ZERO(&mask);
    CPU_SET(3,&mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
	printf("warning: could not set CPU affinity, continuing...\n");
    }

    if (sched_getaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
        perror("get cpu affinity failed.\n");
	return -1;
    } else {
	print_cpu_mask(mask);
    }

    sched_method_cur = sched_getscheduler(0);
    sched_getparam(0, &param);

    printf("sched method/priority: %d/%d\n",sched_method_cur,param.sched_priority);

    if(sched_method == 1 || sched_method == 2) {
	param.sched_priority = sched_priority;
	ret = sched_setscheduler(getpid(),sched_method,&param);
	if(ret)
	{
	    fprintf(stderr,"set scheduler to %d %d failed %m\n");
	    return -4;
	}
    }

    sched_method_cur = sched_getscheduler(0);
    sched_getparam(0, &param);

    fprintf(stderr,"the scheduler of PID(%ld) is %d, priority (%d),BEGIN time(s) is :%ld\n",
	    getpid(),sched_method_cur,param.sched_priority,time(NULL));

    /*
     *for(i = 0;i<COUNT;i++) {
     *    test_func();
     *}
     */

/*
 *    sleep是给其他进程运行的机会,
 *        这里指给其它进程迁移及set fifo和priority的机会.
 *
 *    所以,sleep 之前最好不要执行　test＿funnc() 否则其它进程得不到机会.
 */

    printf("going sleep PID(%ld),time(s) is: %ld \n",getpid(),time(NULL));
    sleep(2);
    sleeptm.tv_sec = 0;
    sleeptm.tv_nsec = NANOSECOND;

    for(i = 0;i<COUNT;i++) {
	test_func();
    }

    interval = MILLION*(tend.tv_sec - tstart.tv_sec)
	       +(tend.tv_usec-tstart.tv_usec);

    fprintf(stderr," PID = %d\t priority: %d\tEND TIME(s) is %ld\n",getpid(),param.sched_priority,time(NULL));
    return 0;
}
