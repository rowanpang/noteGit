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

/*
 *                             lowRebase 50%
 *                bits  3s    |
 *             +-----------+  |  +-----------+
 *             |stable ^ af|  V  |           |
 *sync end|----+       |   +-----+           +-----
 *                     |    low 2s
 *                   sample
 */
//in ms
#define CYCLE 1000
#define CYCLE_REBASE_RATE 40
#define CYCLE_REBASE	(CYCLE*CYCLE_REBASE_RATE/100)

#define SAMP_STABLE_RATE 60	//60%
#define	REBASE_WAIT_RATE 50

#define CYCLE_BITS	(CYCLE - CYCLE_REBASE)
#define CYCLE_BITS_FORK (CYCLE_BITS - differtime)
#define SAMP_STABLE	((CYCLE_BITS+1)*SAMP_STABLE_RATE/100)
#define SAMP_AFTER	(CYCLE_BITS - SAMP_STABLE)

#define REBASE_WAIT ((CYCLE_REBASE+1)*SAMP_STABLE_RATE/100)
#define REBASE_AFTER (CYCLE_REBASE - REBASE_WAIT)


int pcntBase;
int differtime;
#define TOFORK_RATE	100	//100%
#define TOFORK	(pcntBase?(pcntBase*TOFORK_RATE/100):100)
    /*
     *100 5~8ms
     *272 13~15ms
     */
#define DELTA_RATE 10	//20%
#define DELTA	((TOFORK+1)*DELTA_RATE/100)

int msleep(int ms)
{
    return usleep(1000*ms);
}

int psCnt()
{
    int ret;
    DIR *dp;
    struct dirent *dent;
    /*int i, len;*/
    int count = 0;
    regex_t regProc;
    char regErrMsg[64];

    ret = regcomp(&regProc,"^[1-9][0-9]*$",REG_NOSUB);
    if(ret){
	regerror(ret,&regProc,regErrMsg,sizeof(regErrMsg));
        fprintf(stderr,"regcomp error %s\n",regErrMsg);
	ret = -1;
	goto fail;
    }

    if( (dp = opendir("/proc") )== NULL) {
        fprintf(stderr,"opendir proc,error %s\n",strerror(errno));
	ret = -2;
	goto fail;
    }

    while((dent = readdir(dp)) != NULL) {
        if(dent->d_type == DT_DIR) {
	    /*
             *len = strlen(dent->d_name);
             *for( i = 0; dent->d_name[i] != 0; ++i) {
             *    if( ! isdigit(dent->d_name[i])) {
             *        break;
             *    }
             *}
             *if( len == i) {
             *    printf("d_name: %s\n",dent->d_name);
             *    ++count;
             *}
	     */

	    ret = regexec(&regProc,dent->d_name,0,NULL,0);
	    if(!ret){
		count++;
	    }
        }
    }
    /*printf("当前系统运行进程数 %d\n",count);*/
    closedir(dp);

    ret = count;
fail:
    return ret;
}

int lowRebase()
{
    int cnt;
    msleep(REBASE_WAIT);
    cnt = psCnt();
    msleep(REBASE_AFTER);
    return cnt;
}


int bitSend(char bit,int cldSleep){
    int ret;
    int i=0;
    pid_t cpid;

    if(bit){
	for (i = 0; i < TOFORK; ++i) {
	    cpid = fork();
	    if(cpid < 0){
		fprintf(stderr,"regcomp error: %s\n",strerror(errno));
		ret = -1;
		goto FAIL;
	    }else if(cpid ==0){ /*child*/
		msleep(cldSleep);
		exit(0);
	    }
	}
    }else{
	msleep(CYCLE_BITS);
    }
    while(i && bit){
	wait(NULL);
	i--;
    }
    /*msleep(CYCLE_BITS);*/

    ret = 0;
FAIL:
    return ret;
}

void send(char bit){
    static int cnt;
    printf("send %d,cnt:%d\n",bit,cnt++);

    pcntBase = lowRebase();
    bitSend(bit,CYCLE_BITS_FORK);
}

int timebase()
{
    int ret;
    struct timespec tp1,tp2;
    clockid_t clkid = CLOCK_REALTIME;
    int differ;

    pcntBase = lowRebase();

    ret = clock_gettime(clkid,&tp1);
    bitSend(1,CYCLE_BITS);
    ret = clock_gettime(clkid,&tp2);


    differ = (tp2.tv_sec-tp1.tv_sec)*1000 +
	     (tp2.tv_nsec-tp1.tv_nsec+500000)/1000000;

    printf("fork num:%d\n",TOFORK);
    printf("tp1:%ld:%ld\n",tp1.tv_sec,tp1.tv_nsec );
    printf("tp2:%ld:%ld\n",tp2.tv_sec,tp2.tv_nsec );
    printf("differ:%d\n",differ-CYCLE_BITS);

    ret = differ-CYCLE_BITS;
    return ret;
}

void about()
{

    printf("CYCLE,%d\n",CYCLE);
    printf("CYCLE_BITS,%d\n",CYCLE_BITS);
    printf("REBASE_WAIT,%d\n",REBASE_WAIT);
    printf("REBASE_AFTER,%d\n",REBASE_AFTER);
    printf("SAMP_STABLE,%d\n",SAMP_STABLE);
}

int bitDetect()
{
    int cntSmp,cntBase=pcntBase;
    msleep(SAMP_STABLE);
    cntSmp = psCnt();
    msleep(SAMP_AFTER);

    printf("base:%d,sample:%d,delta:%d,tofork:%d\n",cntBase,cntSmp,DELTA,TOFORK);

    if(abs(cntSmp-cntBase)+DELTA > TOFORK){
	return 1;
    }

    return 0;
}

int recive(){
    char val;
    static int cnt;
    pcntBase = lowRebase();
    val = bitDetect();
    printf("recv:%d,cnt:%d\n",val,cnt++);
    return val;
}

void synchronize(int sender)
{
    time_t start = time(NULL);
    printf("---------------Synchronizing start, %ld ...\n",start);

    while(1)
    {
	if(start % 10 == 0){
	    break;
	}

        msleep(300);
        start = time(NULL);
    }

    differtime =  timebase();
    printf("--------------Synchronizing ok, %ld ...\n",start);
}
