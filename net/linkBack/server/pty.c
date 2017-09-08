#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/fcntl.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

int setuplistener(char *ipStr,int port)
{
    int ret;
    int sk;
    struct sockaddr_in addr;		//man 7 ip

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if(!ipStr){
	addr.sin_addr.s_addr = INADDR_ANY;
    }else{
	if((ret = inet_aton(ipStr,&addr.sin_addr)) == 0){
	    dprintf(2,"addr %s fmt error!\n",ipStr);
	    ret = -11;
	    goto ADDRERROR;
	}    
    }

    ret = socket(AF_INET,SOCK_STREAM,0);
    if(ret == -1){
        perror("socket error");
	goto ADDRERROR;
    }
    sk = ret;

    if(bind(sk,(struct sockaddr*)&addr,sizeof(addr))){
        perror("when bind");
        ret = -1;
        goto UNMATCH;
    }
    
    printf("before listen\n");    
    if(listen(sk,8)){
        perror("when listen");
        ret = -2;
        goto UNMATCH;
    }

    return sk;

UNMATCH:
    close(sk);
ADDRERROR:
    return ret;
}

int waitClient(int listener)
{
    int ret;
    int client;
    struct pollfd pollIn;
    struct sockaddr_in cli_addr;
AGAIN:
    pollIn.fd = listener;
    pollIn.events = POLLIN;
    pollIn.revents = 0;
    do{
	printf("--poll start--\n");    
	ret = poll(&pollIn,1,-1);	    /*timeout neative case the poll block*/
    }while(ret <= 0);

    socklen_t cli_addr_size = sizeof(cli_addr);
    if(pollIn.events == POLLIN){
	ret = accept(listener,(struct sockaddr*) &cli_addr,&cli_addr_size);
	if(ret < 0){
	    perror("accept error!");
	    goto ERROR_1;
	}
	client = ret;
	
	printf("cli_addr:%s\n",inet_ntoa(cli_addr.sin_addr));
	printf("cli_addr:%lx\n",(int)(cli_addr.sin_addr.s_addr));
    }else{
	goto AGAIN;
    }

    return client;

ERROR_1:
    return ret;
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

int termiosRaw(int fd,char restore)
{
    int ret;
    static struct termios org;
    static int bakOk = 0;
    struct termios new;

    if (!isatty(fd)){
	ret = errno;
	perror("not a tty file descriptor");
	goto ERROR;
    }
										  
    if (restore && bakOk){
	ret = tcsetattr(fd,TCSAFLUSH,&org);
	goto OK;
    }

    if(ret = tcgetattr(fd,&org)){
	perror("when tcgetattr");
	goto ERROR;	
    }                                                                               

    new = org;
    cfmakeraw(&new);
    if(ret = tcsetattr(fd,TCSAFLUSH,&new)){
	perror("when attr set to raw");
	goto ERROR;	
    }                                                                               
    bakOk = 1;

    ret = 0;

ERROR:
OK:
    return ret;
}
    
int main(void)
{
    int ret;
    int listener;
    int client;
    int port = 1066;

    /*signal(SIGINT,ctrl_c_handler);*/
    ret = setuplistener(NULL,port);
    if(ret < 0){
	goto LISTENERROR;
    }
    listener = ret;

    ret = waitClient(listener);
    if(ret < 0){
	goto NOCLIENT;
    }
    client = ret;

    termiosRaw(STDIN_FILENO,0);
    while(1){
	ret = bidirection(STDIN_FILENO,STDOUT_FILENO,client,client);
	if (ret < 0){
	    goto BIDERROR;
	}
    }

    ret = 0;
BIDERROR:
    termiosRaw(STDIN_FILENO,1);
    close(client);

NOCLIENT:
    close(listener);
LISTENERROR:
    ret = -1;
    return ret;
}
