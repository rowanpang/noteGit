#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>

#define BUFSIZE 1024
int ctrl_c=0;
void ctrl_c_handler(int signum){
    printf("handler for ctrl-c\n");
    ctrl_c=1;
}

int threadreadexit = 0;
void *readandsendtosk(void *skp)
{
    int sk = *(int *)skp;
    int ret;
    char buf[BUFSIZE];
    struct termios orgtcattr,newtcattr;

    if(ret = tcgetattr(STDIN_FILENO,&orgtcattr)){
        perror("when tcgetattr");
    }

    newtcattr = orgtcattr;
    cfmakeraw(&newtcattr);
    if(ret = tcsetattr(STDIN_FILENO,TCSAFLUSH,&newtcattr)){
        perror("when tcgetattr");
    }

    while(1){
        bzero(buf,BUFSIZE);
        /*printf("start read");*/
        /*fflush(stdout);*/
        ret = read(STDIN_FILENO,buf,BUFSIZE);
        ret=send(sk,buf,ret,0);
        if(ret == -1){
            perror("when send");
            ret = -4;
        }

        if(threadreadexit){
            break;
        }
    }

    tcsetattr(STDIN_FILENO,TCSAFLUSH,&orgtcattr);
    pthread_exit(NULL);
}

int threadrecvexit = 0;
void *recvskandprint(void *skp)
{
    int sk = *(int *)skp;
    int ret;
    char buf[BUFSIZE];
    do{
        bzero(buf,BUFSIZE);
        ret = recv(sk, &buf, sizeof(buf), 0);    
        if(ret == -1){
            perror("when recv");
            ret = -3;
        }else{
            write(STDOUT_FILENO,buf,ret);
        }
    
        if(threadrecvexit){
            break;
        }
    }while(1);
    pthread_exit(NULL);
}

int setuplistener(char *ipStr,int port)
{
    int ret;
    int listener;
    struct sockaddr_in addr;		//man 7 ip

    addr.sin_family = AF_INET;
    addr.sin_port = (port == 0) ? htons(8080) : htons(port);
    if(!ipStr){
	addr.sin_addr.s_addr = INADDR_ANY;
    }else{
	if((ret = inet_aton(ipStr,&addr.sin_addr)) == 0){
	    dprintf(2,"addr %s fmt error!\n",ipStr);
	    ret = -11;
	    goto ERROR_1;
	}    
    }

    ret = socket(AF_INET,SOCK_STREAM,0);
    if(ret == -1){
        perror("socket error");
	goto ERROR_1;
    }
    listener = ret;

    if(bind(listener,(struct sockaddr*)&addr,sizeof(addr))){
        perror("when bind");
        ret = -1;
        goto ERROR_2;
    }
    
    printf("before listen\n");    
    if(listen(listener,8)){
        perror("when listen");
        ret = -2;
        goto ERROR_2;
    }

    return listener;

ERROR_2:
    close(listener);
ERROR_1:
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
	ret = poll(&pollIn,1,100);
	if(ctrl_c){
	    exit(0);
	}
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
    
int main(void)
{
    int ret;
    int listener;
    int client;
    pthread_t thr_send,thr_recv;

    signal(SIGINT,ctrl_c_handler);
    ret = setuplistener(NULL,0);
    if(ret < 0){
	goto ERROR_1;
    }
    listener = ret;

    ret = waitClient(listener);
    if(ret < 0){
	goto ERROR_2;
    }
    client = ret;

    pthread_create(&thr_send,NULL,readandsendtosk,&client);
    pthread_create(&thr_recv,NULL,recvskandprint,&client);
    while(1){
	if(ctrl_c){
	    printf("terminal by ctrl-c\n");
	    threadrecvexit = 1;
	    threadreadexit = 1;
	    pthread_join(thr_send,NULL);
	    pthread_join(thr_recv,NULL);
	}
    }

    ret = 0;
    close(client);
ERROR_2:
    close(listener);
ERROR_1:
    return ret;
}

