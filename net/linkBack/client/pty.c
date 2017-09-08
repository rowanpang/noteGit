#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <pty.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/un.h>

#define BUFSIZE 1024
int port = 1066;

int connectServer()
{
    int sk;
    int ret;
    struct sockaddr_in svrAddr={0};

    sk = socket(AF_INET,SOCK_STREAM,0);

    svrAddr.sin_family = AF_INET;
    svrAddr.sin_port = htons(port);
    inet_aton("127.0.0.1",&svrAddr.sin_addr);

    printf("before connect\n");    
    if(connect(sk,(struct sockaddr*) &svrAddr,sizeof(svrAddr))){
        printf("connect error: %s,return\n",strerror(errno));    
        ret = errno;
        goto OUT;
    }

    ret = sk;
OUT:
    return ret;
}

int openPtmx(int* master,int *slave)
{
    int ret;
    char *slavePath;

    ret = open("/dev/ptmx",O_RDWR|O_NOCTTY);    //equal to getpt() man openpty
    if(ret == -1){
        perror("ptmx open error");
	return -1;
    }
    *master = ret;

    ret = grantpt(*master);
    if(ret != 0){
        perror("grantpt error!");
        goto CLOSE_MASTER;
    }
    ret = unlockpt(*master);
    if(ret != 0){
        perror("unlockpt error!");
        goto CLOSE_MASTER;
    }

    slavePath = ptsname(*master);    
    printf("new ptmx paire: %d,%s\n",*master,slavePath);
    ret = open(slavePath,O_RDWR);
    if(ret == -1){
        perror("slave open error");
        goto CLOSE_MASTER;
    }
    *slave = ret;
    return ret;

CLOSE_MASTER:
    close(*master);
    return ret;
}

void confirmReady()
{
    int ret;
    char c;
    char buf[10]={0};
    do{
        printf("start recv (yes/no)[yes]:\n");
        #define READ_SIZE 4
        ret = read(STDIN_FILENO,buf,READ_SIZE);
        if(ret != -1){
            if(!strncmp("no",buf,2)){
                /*do nothing*/
            }else if(!strncmp("yes",buf,3)){
                break;
            }else if(!strncmp("\n",buf,1)){
                break;
            }
        }
        printf("ret %d\n",ret);
        if((ret == READ_SIZE) && (buf[READ_SIZE - 1] != '\n')){
            printf("flush\n");
            while ((c=getchar()) != '\n' && c != EOF);
        }
    } while(1);
}

void childExec(int ptm,int pts)
{
    /*child 将pty的slave做为stdin/out/err.*/
    char * argv[] = {"/bin/bash","-i",NULL};
    printf("child shell\n");

    dup2(pts,STDIN_FILENO);
    dup2(pts,STDOUT_FILENO);
    dup2(pts,STDERR_FILENO);

    int maxfd=sysconf(_SC_OPEN_MAX);
    for(int fd=3; fd<maxfd; fd++){
	close(fd);
    }

    setsid();                                                                       
    ioctl(0,TIOCSCTTY,1);

    execv(argv[0],argv);
}

#define MAX(a,b) (a)>(b)?(a):(b)
int bidirection(int li,int lo,int ri,int ro)
{
    int buf[4096];
    int ret;
    int fdIn,fdOut;
    int maxfd = MAX(MAX(li,lo),MAX(ri,ro));
    fd_set fdsr;

    /*printf("li:%d,lo:%d,ri:%d,ro:%d\n",li,lo,ri,ro);*/

    FD_ZERO(&fdsr);
    FD_SET(li,&fdsr);        //0
    FD_SET(ri,&fdsr);

    /*printf("maxfd:%d\n",maxfd);*/
    ret = select(maxfd+1,&fdsr,NULL,NULL,NULL);
    if(ret == -1){
        perror("select error");
        goto ERROR;
    }

    fdIn = STDIN_FILENO;
    fdOut = STDOUT_FILENO;
    if(FD_ISSET(li,&fdsr)){
        fdIn = li;
        fdOut = ro;
    }else if(FD_ISSET(ri,&fdsr)){
        fdIn = ri;
        fdOut = lo;
    }else {
        printf("none match\n");
        goto UNMATCH;
    }

    bzero(buf,sizeof(buf));
    ret = read(fdIn,buf,sizeof(buf));
    if(ret < 0){
        perror("read stdin error");
        goto READERR;
    }else if(ret == 0){
        printf("read size 0\n");
        goto IEOF;
    }

    ret = write(fdOut,buf,ret);     //send stdin to master.
    if(ret == 0){
        perror("write out error");
    }

OK:
    return ret;

ERROR:
READERR:
UNMATCH:
IEOF:
    ret = -1;

    return ret;
}

int main(int argc,char **argv)
{
    int ret;
    int sk;
    int ptm;
    int pts;
    int cpid;

    ret = connectServer();
    if(ret < 0){
	goto ERROR_SK;
    }
    sk = ret;
    /*ret = openPtmx();*/
    ret = openpty(&ptm,&pts,NULL,NULL,NULL);
    if(ret < 0){
	goto ERROR_PTMX;
    }

    ret = fork();
    if(ret == 0){
	childExec(ptm,pts);
    }else if(ret < 0){
	goto ERROR_PTMX;
    }

    close(pts);
    while(1){
	ret = bidirection(ptm,ptm,sk,sk);
	if (ret < 0){
	    goto BIDERROR;
	}
    }

    cpid = ret;
    waitpid(cpid,&ret,0);

    ret = 0;
BIDERROR:
ERROR_PTMX:
ERROR_SK:
    return ret;
}
