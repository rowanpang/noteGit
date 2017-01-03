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
    newtcattr.c_lflag &=  (~(ECHO | ICANON));
    /*newtcattr.c_lflag &=  (~(ICANON));*/
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

//man 7 ip for more info
int main(void)
{
    int sk_listen;
    int ret;
    struct sockaddr_in svr_addr={0};
    unsigned port_listen = 8080;

    signal(SIGINT,ctrl_c_handler);

    sk_listen = socket(AF_INET,SOCK_STREAM,0);
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(port_listen);
    svr_addr.sin_addr.s_addr = INADDR_ANY;
    /*
     *if(inet_aton("192.168.137.100",&svr_addr.sin_addr) == 0){
     *    perror("when valued addr");
     *    ret = -11;
     *    goto out;
     *}    
     */

    if(bind(sk_listen,(struct sockaddr*)&svr_addr,sizeof(svr_addr))){
        perror("when bind");
        ret = -1;
        goto out;
    }
    
    printf("before listen\n");    
    if(listen(sk_listen,8)){
        perror("when listen");
        ret = -2;
        goto out;
    }

    struct pollfd svr_poll;
    svr_poll.fd = sk_listen;
    svr_poll.events = POLLIN;
    svr_poll.revents = 0;
    
    while(1){
        printf("before poll\n");    
        int nr=0;
        do{
            nr = poll(&svr_poll,1,100);
            if(ctrl_c){
                printf("terminal by ctrl-c\n");
                ret = 0;
                goto out;
            }
        }while(nr <= 0);

        int sk;
        struct sockaddr_in cli_addr;
        socklen_t cli_addr_size = sizeof(cli_addr);
        if(svr_poll.events == POLLIN){
            sk = accept(sk_listen,(struct sockaddr*) &cli_addr,&cli_addr_size);
            printf("cli_addr:%s\n",inet_ntoa(cli_addr.sin_addr));
            printf("cli_addr:%lx\n",(int)(cli_addr.sin_addr.s_addr));
        }    
        char buf[BUFSIZE] = {0};
        pthread_t thr_send,thr_recv;
        pthread_create(&thr_send,NULL,readandsendtosk,&sk);
        pthread_create(&thr_recv,NULL,recvskandprint,&sk);
        while(1){
            if(ctrl_c){
                printf("terminal by ctrl-c\n");
                threadrecvexit = 1;
                threadreadexit = 1;
                pthread_join(thr_send,NULL);
                pthread_join(thr_recv,NULL);
                close(sk);
                ret = 0;
                goto out;
            }
        }

    next:
        close(sk);
        printf("\n");
        if(!ret){
            printf("client connection failed\n");
            goto out;
        }

    }
    ret = 0;
out:
    close(sk_listen);

    return ret;
}

