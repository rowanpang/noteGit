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

#define TIME_ESCAPE_CAL 1

int chdToFork = 100;
int chdForked = 0;
int *cpids;

int pollOpen()
{
    /*int fds[1000];*/
    int ret;
    int i,j;
#if TIME_ESCAPE_CAL
    struct timeval tvlast,tvNow,tvRes;
    gettimeofday(&tvlast,NULL);
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

#if TIME_ESCAPE_CAL
    gettimeofday(&tvNow,NULL);
    timersub(&tvNow,&tvlast,&tvRes);
    fprintf(stderr,"diff1:%ld,%ld\n",tvRes.tv_sec,tvRes.tv_usec);
#endif

    sleep(10);

#if TIME_ESCAPE_CAL
    gettimeofday(&tvlast,NULL);
#endif

    while(chdForked--){
    	//printf("kill and wait cpid:%d\n",cpids[chdForked]);
    	kill(cpids[chdForked],SIGKILL);
    	waitpid(cpids[chdForked],NULL,0);
    }
#if TIME_ESCAPE_CAL
    gettimeofday(&tvNow,NULL);
    timersub(&tvNow,&tvlast,&tvRes);
    sleep(1);
    printf("diff1:%ld,%ld\n",tvRes.tv_sec,tvRes.tv_usec);
#endif
    return 0;
}

int main(int argc,char **argv)
{
	cpids = malloc(chdToFork*sizeof(int));
	pollOpen();
	return 0;
}

