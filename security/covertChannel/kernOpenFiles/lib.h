#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/*
 *            TIME_TO_SAMPLE
 *                 |
 *                 |   TIME_TO_REBASE
 *                 V       |
 *               -----     |    ------
 *              /    ^\    V   /      \
 *sync end|----/     | \------/        \----
 *             ^     |
 *             |  TIME_TO_CHILDEXIT
 *             |
 *        TIME_TO_TRIGER
 */

//#define  TIME_DEBUG 0

/* fork ctx time
 *100 5~8ms
 *272 13~15ms
 */
//in ms
#define CYCLE 			200
#define TIME_TO_LOW		0
#define TIME_TO_TRIGER		10
#define TIME_TO_SAMPLE		120
#define TIME_TO_CHILDEXIT	140
#define TIME_TO_REBASE		190

#define SAMP_STABLE_RATE 60	//60%
#define	REBASE_WAIT_RATE 50

#define CYCLE_BITS	(TIME_TO_CHILDEXIT - TIME_TO_TRIGER)
#define CYCLE_BITS_FORK (CYCLE_BITS)

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
    clockid_t clkid = CLOCK_REALTIME;

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

int smpBaseVal;
int smpRebase()
{
    int ret;
    waitfor(TIME_TO_REBASE);
    ret = open("/dev/zero",O_RDONLY);
    if(ret > 0){
	smpBaseVal = 1;
	//printf("rebase open ok\n");
    }else{
	smpBaseVal = 0;
    }
    close(ret);

    return ret;
}

int chdToFork = 100;
int chdForked = 0;
int *cpids;

int pollOpen()
{
    /*int fds[1000];*/
    int ret;
    int i,j;

#ifdef TIME_DEBUG
    gettimeofday(&tvTmp,NULL);
    printf("pollOpen:%ld,%ld\n",tvTmp.tv_sec,tvTmp.tv_usec);
#endif

    for(i=0;i<chdToFork;i++){
        ret = fork();
        if(ret == 0){
            //printf("curpid:%d,ppid:%d\n",getpid(),getppid());
            for(j=0;j<1000;j++){
                ret = open("/dev/zero",O_RDONLY);
                if(ret <0){
                    //perror("when open:");
                }else{
                    //fds[j] = ret;
                }
            }
            pause();
            exit(0);
        }else if (ret > 0){
            cpids[i] = ret;
	    chdForked++;
        }else{
            perror("fork error");
            exit(0);
        }
    }

    return 0;
}

int bitSend(char bit,int cldSleep){
    int ret;

    waitfor(TIME_TO_TRIGER);
    if(bit){
	pollOpen();
    }else{

    }

    waitfor(TIME_TO_CHILDEXIT);

#ifdef TIME_DEBUG
    gettimeofday(&tvTmp,NULL);
    printf("childToexit:%ld,%ld\n",tvTmp.tv_sec,tvTmp.tv_usec);
#endif
    if(bit){
	while(chdForked--){
	    //printf("kill and wait cpid:%d\n",cpids[chdForked]);
	    kill(cpids[chdForked],SIGKILL);
	    waitpid(cpids[chdForked],NULL,0);
	}
    }

    ret = 0;
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

    waitfor(TIME_TO_SAMPLE);
#ifdef TIME_DEBUG
    gettimeofday(&tvTmp,NULL);
    printf("bitDetect:%ld,%ld\n",tvTmp.tv_sec,tvTmp.tv_usec);
#endif

    ret = open("/dev/zero",O_RDONLY);
    //printf("detect open ret:%d\n",ret);
    if(ret > 0){
	//open ok,so recv '0'
	close(ret);
	ret = 0;
    }else if (ret == -1){
	//perror("open:");
	if (errno == ENFILE){
	    //system-wide limit reached recv '1'
	    ret = 1;
	}
    }
    return ret;
}

int recive(){
    char val;
    static int cnt;
    smpBaseVal = smpRebase();
    val = bitDetect();
    printf("recv:%d,cnt:%d\n",val,cnt++);
    return val;
}

void synchronize(int sender)
{
    time_t start = time(NULL);
    printf("---------------Synchronizing start, %ld ...\n",start);
    if(sender){
	cpids = malloc(chdToFork*sizeof(int));
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
