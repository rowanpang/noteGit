#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <ctype.h>
#include <errno.h>
#include <regex.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sched.h>

/*
 *            TIME_TO_SAMPLE
 *                 |
 *                 |   TIME_TO_REBASE
 *                 V       |
 *               -----     |    ------
 *              /    ^\    V   /      \
 *sync end|----/     | \------/        \----
 *             ^     |
 *             |  TIME_TO_UNTRIGER
 *             |
 *        TIME_TO_TRIGER
 */

/*fork ctx time
 *100 5~8ms
 *272 13~15ms
 */
//in ms
#define CYCLE 			100
#define TIME_TO_LOW		0
#define TIME_TO_TRIGER		(CYCLE*5/100)
#define TIME_TO_SAMPLE		(CYCLE*60/100)
#define TIME_TO_UNTRIGER	(CYCLE*70/100)
#define TIME_TO_REBASE		(CYCLE*95/100)

#define CYCLE_BITS	(TIME_TO_UNTRIGER - TIME_TO_TRIGER)
#define CYCLE_BITS_FORK (CYCLE_BITS)

#define CPU_AFFINITY 0
#define CLOCKID CLOCK_REALTIME
#define POSIX_TIMER_FUNC "posix_timer_fn"
#define NSEC_PER_SEC 1000000000
#define TIMER_INDENTIFY (1<<30)


//#define  TIME_DEBUG 1
#ifdef TIME_DEBUG
struct timeval tvTmp;
#endif

int msleep(int ms)
{
    return usleep(1000*ms);
}

int waitfor(int point)
{
    struct timespec tp;
    int now;
    int deltal = 3,deltar = 3;
    int ret;
    clockid_t clkid = CLOCKID;

    switch(point){
	case TIME_TO_SAMPLE:
	    deltal = 0;
	    break;
	default:
	    break;
    }

    ret = clock_gettime(clkid,&tp);
    now = (tp.tv_nsec/1000000)%CYCLE;
    while(1){
	if(point - deltal < now && now < point + deltar){
	    break;
	}
	msleep(1);
	ret = clock_gettime(clkid,&tp);
	now = (tp.tv_nsec/1000000)%CYCLE;
    }

    return ret;
}

int smpRebase()
{
    int ret;
    waitfor(TIME_TO_REBASE);

    ret = 0;
    return ret;
}

void tkNotify(union sigval tkval)
{
}

int mkTimer(timer_t *tid)
{
    int ret;
    timer_t tk;
    struct sigevent sev;
    struct itimerspec its;
    clockid_t clkid = CLOCKID;

    bzero(&sev,sizeof(sev));
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = tkNotify;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = &tk;

    ret = timer_create(clkid,&sev,&tk);
    if(ret == -1){
        perror("when timer create");
        goto OUT;
    }

    its.it_value.tv_sec = TIMER_INDENTIFY;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;
    ret = timer_settime(tk,0,&its,NULL);
        /*flag为0,2s 后触发timer.*/
    if(ret == -1){
        perror("when timer settime");
        goto OUT;
    }

    *tid = tk;
    ret = 0;
OUT:
    return ret;
}

int bitSend(char bit,int cldSleep){
    int ret;
    timer_t tk;

    waitfor(TIME_TO_TRIGER);
#ifdef TIME_DEBUG
    gettimeofday(&tvTmp,NULL);
    printf("bitOne before:%ld,%ld\n",tvTmp.tv_sec,tvTmp.tv_usec);
#endif
    if(bit){
	ret = mkTimer(&tk);
	if(ret < 0){
	    goto OUT;
	}
    }else{

    }
#ifdef TIME_DEBUG
    gettimeofday(&tvTmp,NULL);
    printf("bitOne after:%ld,%ld\n",tvTmp.tv_sec,tvTmp.tv_usec);
#endif

    waitfor(TIME_TO_UNTRIGER);

#ifdef TIME_DEBUG
    gettimeofday(&tvTmp,NULL);
    printf("untriger:%ld,%ld\n",tvTmp.tv_sec,tvTmp.tv_usec);
#endif

    if(bit){
        ret = timer_delete(tk);
	if(ret < 0){
	    goto OUT;
	}
    }

    ret = 0;

OUT:
    return ret;
}

void send(char bit){
    static int cnt;
    smpRebase();
    bitSend(bit,CYCLE_BITS_FORK);
    printf("send %d,cnt:%d\n",bit,cnt++);
}

void about()
{

}

int bitDetect()
{
    int ret;
    FILE *stream;
    char *line;
    size_t len=0;
    ssize_t n;

    int cpuid;
    char gotCpu = 0;

    int clkid;
    char gotClk = 0;

    char fnName[128];

    unsigned long soft,expire,now;

    waitfor(TIME_TO_SAMPLE);
#ifdef TIME_DEBUG
    gettimeofday(&tvTmp,NULL);
    printf("bitDetect:%ld,%ld\n",tvTmp.tv_sec,tvTmp.tv_usec);
#endif

    stream = fopen("/proc/timer_list","r");

    while(1){
	n = getline(&line,&len,stream);
	if(n == -1){
	    ret = errno;
	    goto OUT;
	}
	/*printf("read:%ld byts\n",n);*/
	if(!strncmp(line,"now at",6)){
	    sscanf(line,"now at %ld nsecs",&now);
	    //printf("now:%ld\n",now);
	}
	if(!strncmp(line,"cpu:",4)){
	    sscanf(line,"cpu: %d\n",&cpuid);
	    //printf("cpuid:%d\n",cpuid);
	    if(cpuid == CPU_AFFINITY){
		gotCpu = 1;
	    }else{
		if(gotCpu){
		    gotCpu = 0;
		    break;
		}
	    }
	}

	if(gotCpu){
	    if(!strncmp(line," clock",6)){
		sscanf(line," clock %d:\n",&clkid);
		//printf("clkid:%d\n",clkid);
		if(clkid == CLOCKID){
		    gotClk = 1;
		}else{
		    if(gotClk){
			gotClk = 0;
			break;
		    }
		}
	    }

	    if(gotClk){
		if(!strncmp(line," #",2)){
		    sscanf(line," #%*d: <%*x>, %[^,]s, S:%*d\n",fnName);
		    if(!strncmp(fnName,POSIX_TIMER_FUNC,strlen(POSIX_TIMER_FUNC))){
			//printf("got func:%s\n", fnName);
			n = getline(&line,&len,stream);
			if(n == -1){
			    ret = errno;
			    goto OUT;
			}
			sscanf(line," # expires at %ld-%ld nsecs [in",&soft,&expire);
			//printf("%ld,%ld\n",soft,expire);
			if((soft/NSEC_PER_SEC)/100 >= TIMER_INDENTIFY/100){
			    /*
			     *此处忽略了timer设置时的soft上＋的'now'值, soft人为设置为'1<<30'
			     *    大概为34年,忽略了系统连续运行的一小段时间.
			     *如果要精确,则应将hrtimer模式更换为ABS,同时设置一个未来的绝对时间
			     */

			    /*
			     *comme here got timer delay for TIMER_INDENTIFY,belong to cpuid->clkid.
			     */
			    //printf("got break\n");
			    ret += 1;
			    break;
			}
		    }
		}
	    }

	}
    }

OUT:
    return ret;
}

int recive(){
    char val;
    static int cnt;
    smpRebase();
    val = bitDetect();
    printf("recv:%d,cnt:%d\n",val,cnt++);
    return val;
}

void synchronize(int sender)
{
    time_t start = time(NULL);
    printf("---------------Synchronizing start, %ld ...\n",start);
    if(sender){
    }

    while(1)
    {
	if(start % 10 == 0){
	    break;
	}

        msleep(1);
        start = time(NULL);
    }

    printf("--------------Synchronizing ok, %ld ...\n",start);
}
