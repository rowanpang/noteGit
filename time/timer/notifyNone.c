#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sched.h>

#include <strings.h>
#define TIMER_INDENTIFY (1<<30)

pid_t gettid()
{
    return syscall(SYS_gettid);
}

void tkNotify(union sigval tkval)
{
    static int count=0;
    int ret;

    printf("%d,in tkNotify,tid:%d,tgid:%d,ppid:%d\n",count,gettid(),getpid(),getppid());

    if(count++ >= 5){
        printf("count ok ,exit\n");
        ret = timer_delete(*(timer_t*)tkval.sival_ptr);
        if(ret == -1){
            perror("when timer delete");
        }
        exit(0);
    }
}

int main(int argc,char **argv)
{
    int ret;
    int i;

    timer_t tk;
    struct sigevent sev;
    struct itimerspec its;
    cpu_set_t cpuMask;

    clockid_t clkid;
    clkid = CLOCK_REALTIME;

    CPU_ZERO(&cpuMask);
    CPU_SET(0,&cpuMask);
    ret = sched_setaffinity(0,sizeof(cpuMask),&cpuMask);
    if(ret == -1){
        perror("when sched_setaffinity");
        goto OUT;
    }

    ret = sched_getaffinity(0,sizeof(cpuMask),&cpuMask);
    if(ret == -1){
        perror("when sched_getaffinity");
        goto OUT;
    }else{
	printf("affinity to:");
	for(i=0;i<4;i++){
	    if(CPU_ISSET(i,&cpuMask)){
		printf("%d ",i);
	    }
	}
	printf("\n");
    }

    printf("handler:%p,%ld\n", tkNotify,(long)tkNotify);
    printf("in main,tid:%d,tgid:%d,ppid:%d\n",gettid(),getpid(),getppid());

    bzero(&sev,sizeof(sev));
    /*
     *sev.sigev_notify = SIGEV_SIGNAL;
     *sev.sigev_signo = SIGALRM;
     *sev.sigev_value.sival_int = 10;
     */

    /*
     *sev.sigev_notify = SIGEV_THREAD;
     *sev.sigev_notify_function = tkNotify;
     *sev.sigev_notify_attributes = NULL;
     *sev.sigev_value.sival_ptr = &tk;
     */

    sev.sigev_notify = SIGEV_NONE;
	/*not show in /proc/timer_list*/
    ret = timer_create(clkid,&sev,&tk);
    if(ret == -1){
        perror("when timer create");
        goto OUT;
    }

    its.it_value.tv_sec = TIMER_INDENTIFY;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;

    printf("id:%p,%ld,press Enter to start timer\n",tk,(long)tk);
    getchar();
    ret = timer_settime(tk,0,&its,NULL);
        /*flag为0,2s 后触发timer.*/
    if(ret == -1){
        perror("when timer settime");
        goto OUT;
    }
    printf("timer started...\n");

    sleep(1); /*coWork well*/

    ret = timer_gettime(tk,&its);
    if(ret == -1){
        perror("when timer gettime");
        goto OUT;
    }else{
	printf("get remain :%ld\n",its.it_value.tv_sec);
    }

    pause();

    ret = 0;
OUT:
    return ret;
}
