#define _GNU_SOURCE
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

#include "lib.h"

int main(int argc, char *argv[])
{
    int ret;
    int cnt = 0;
    int i;

    cpu_set_t cpuMask;

    CPU_ZERO(&cpuMask);
    CPU_SET(CPU_AFFINITY,&cpuMask);
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
        printf("affinity to cpu:");
        for(i=0;i<4;i++){
	    if(CPU_ISSET(i,&cpuMask)){
                printf("%d ",i);
	    }
        }
        printf("\n");
    }

    about();
    synchronize(1);
    while(1){
	if(cnt & 0x01){
	    send(1);
	}else{
	    send(0);
	}
	//if(cnt) break;
	cnt++;
    }

    ret = 0;
OUT:
    return ret;
}
