#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>

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

    timer_t tk;
    struct sigevent sev;
    struct itimerspec its;


    printf("in main,tid:%d,tgid:%d,ppid:%d\n",gettid(),getpid(),getppid());
    /*
     *sev.sigev_notify = SIGEV_SIGNAL;
     *sev.sigev_signo = SIGALRM;
     *sev.sigev_value.sival_int = 10;
     */

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = tkNotify;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = &tk;

    ret = timer_create(CLOCK_REALTIME,&sev,&tk);
    if(ret == -1){
        perror("when timer create");
        goto OUT;
    }

    its.it_value.tv_sec = 5;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;

    printf("press Enter to start timer\n");
    getchar();
    ret = timer_settime(tk,0,&its,NULL);
        /*flag为0,2s 后触发timer.*/
    if(ret == -1){
        perror("when timer settime");
        goto OUT;
    }

    pause();

    ret = 0;
OUT:
    return ret;
}
