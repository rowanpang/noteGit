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

    /*
     *signal(SIGINT,ctrl_c_handler);
     */

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

    int sk;
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_size = sizeof(cli_addr);
    sk = accept(sk_listen,(struct sockaddr*) &cli_addr,&cli_addr_size);
    printf("accpet\n");
    printf("	cli_addr:%s\n",inet_ntoa(cli_addr.sin_addr));
    printf("	cli_addr:%lx\n",(int)(cli_addr.sin_addr.s_addr));

    char buf[100];
    while(1){
	bzero(buf,100);
        ret=recv(sk, buf,sizeof(buf),0);
        if(ret == -1){
            perror("when recv");
            ret = -3;
            goto out;
        }else{
            printf("recv %d bytes,%s\n",ret,buf);
        }
    }
    ret = 0;
out:
    close(sk_listen);

    return ret;
}

