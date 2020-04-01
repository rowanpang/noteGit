#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>


int pipefd[2];
int cpids[1024];

int forknum = 5;
int dbg = 0;

#define dbg(fmt,arg...)\
    if (dbg){\
	printf("func: %s,"fmt"\n",__FUNCTION__,##arg);\
    }

char *ucargs = NULL;
int childrun()
{
    int ret;
    char *args[] = {"bashdef","5",NULL};
    char **eargs = args;
    char buf[5];
    char uargcnt=0;
    char *tmp;
    char *cargs[10+2];
    char cucargs[256];
    int i=0;

    char *binelf="./hello";
    char *procname="bashhello";

    if ( ucargs ){
	tmp = strcpy(cucargs,ucargs);	//no need cpy, fork will do cow
	dbg("child,ucargs :%s\n",tmp);

	uargcnt = 1;//arg0 for exec name,so skip 0.
	cargs[uargcnt] = tmp;
	uargcnt++;
	while ((tmp = strchr(tmp,' ')) != NULL){
	    *tmp = '\0';
	    tmp++;
	    dbg("tmp1:%d,%#x\n",*tmp,*tmp);
	    while (isspace((int)*tmp)){
		tmp++;
		dbg("tmp:%d,%#x\n",*tmp,*tmp);
	    }
	    cargs[uargcnt] = tmp;
	    uargcnt++;
	}

	dbg("pid:%d ,uargcnt: %d\n",getpid(),uargcnt);
	if (dbg)
	    for (i = 1; i < uargcnt; i++)
		printf("pid:%d ,uarg: %d, %s\n",getpid(),i,cargs[i]);

	cargs[0] = procname;
	cargs[uargcnt] = NULL;	//exec arg terminate

	eargs = cargs;
    }

    close(pipefd[1]);
    dbg("childrun before read\n");
    read(pipefd[0],buf,4);	//"sync"

    dbg("childrun do execv\n");
    ret = execv(binelf,eargs);

    perror("when execv");   //should not be here;
    return ret;
}


int main(int argc,char** argv)
{
    int ret;
    int i;
    pid_t cpid;
    if  (argc != 2 && argc != 3) {
	printf("usage %s [thsnum,<%d>]\n",argv[0],forknum);
	exit(-1);
    }

    if (argc == 3) {
	ucargs = argv[2];
	printf("usargs: <%s>\n",ucargs);
    }

    forknum=atoi(argv[1]);

    pipe(pipefd);

    for (i = 0; i < forknum ; i++) {
	dbg("forkchild :%d, ",i);
	cpid = fork();
	if(!cpid){
	    /*child*/
	    childrun();
	}else if(cpid != -1){
	    cpids[i] = cpid;
	    dbg("cpid: %d\n",cpid);
	}else{
	    perror("when fork");
	    ret = -1;
	    goto out;
	}
    }

    close(pipefd[0]);
    usleep(1000*500);
    for(i = 0; i < forknum ; i++){
	dbg("write pipe %d\n",i);
	write(pipefd[1],"sync",4);
    }
    sleep(5);
    //kill(0,SIGINT);
    for(i = 0; i < forknum ; i++){
	kill(cpids[i],SIGINT);
	usleep(1);
    }

    int cstatus;

    for(i = 0; i < forknum ; i++){
	dbg("starting wait child :%d\n",i);
	waitpid(cpids[i],&cstatus,0);
	dbg("child exit pid:%d,%d\n",cpids[i],cstatus);
    }

    ret = 0;
out:
    printf("father exit\n");
    return ret;
}
