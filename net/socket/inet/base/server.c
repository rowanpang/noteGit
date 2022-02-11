#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<sys/poll.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<unistd.h>

int ctrl_c=0;
void ctrl_c_handler(int signum){
    printf("handler for ctrl-c\n");
    ctrl_c=1;
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
    /*if(inet_aton("192.168.137.100",&svr_addr.sin_addr) == 0){*/
        /*perror("when valued addr");*/
        /*ret = -11;*/
        /*goto out;*/
    /*}    */

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
            printf("cli_addr:%lx, %s:%d\n",(int)(cli_addr.sin_addr.s_addr),inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
        }

        char buf[100] = {0};
        ret=recv(sk, &buf, sizeof(buf), 0);
        if(ret == -1){
            perror("when recv");
            ret = -3;
            goto next;
        }else{
            printf("recv %d bytes,%s\n",ret,buf);
        }
        bzero(buf,100);
        strcpy(buf,"from service-->client");
        ret=send(sk,buf,strlen(buf),0);
        if(ret == -1){
            perror("when send");
            ret = -4;
            goto next;
        }
    next:
        close(sk);
        printf("\n");
        if(!ret){
            printf("client connection failed\n");
            goto out;
        }
        /*if(ctrl_c){*/
            /*printf("terminal by ctrl-c\n");*/
            /*ret = 0;*/
            /*goto out;*/
        /*}*/
    }
    ret = 0;
out:
    close(sk_listen);

    return ret;
}

