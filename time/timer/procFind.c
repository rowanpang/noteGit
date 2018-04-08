#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>

#define CPU_AFFINITY 0
#define CLOCKID 0
#define FUNC_NAME "posix_timer_fn"
#define NSEC_PER_SEC 1000000000
#define TIMER_INDENTIFY (1<<30)

int main(int argc,char **argv)
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
	    printf("now:%ld\n",now);
	}
	if(!strncmp(line,"cpu:",4)){
	    sscanf(line,"cpu: %d\n",&cpuid);
	    printf("cpuid:%d\n",cpuid);
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
		printf("clkid:%d\n",clkid);
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
		    if(!strncmp(fnName,FUNC_NAME,strlen(FUNC_NAME))){
			printf("got func:%s\n", fnName);
			n = getline(&line,&len,stream);
			if(n == -1){
			    ret = errno;
			    goto OUT;
			}
			sscanf(line," # expires at %ld-%ld nsecs [in",&soft,&expire);
			printf("%ld,%ld\n",soft,expire);
			if((soft/NSEC_PER_SEC)/100 >= TIMER_INDENTIFY/100){
			    /*
			     *此处忽略了timer设置时的soft上＋的'now'值, soft人为设置为'1<<30'
			     *    大概为34年,忽略了系统连续运行的一小段时间.
			     *如果要精确,则应将hrtimer模式更换为ABS,同时设置一个未来的绝对时间
			     */
			    printf("got break\n");
			    break;
			}
		    }
		}
	    }

	}
    }

    ret = 0;
OUT:
    return ret;
}
