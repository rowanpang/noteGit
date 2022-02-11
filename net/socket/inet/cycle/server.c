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
#include<strings.h>

void dorecv(int sk)
{
    char buf[100];
    int ret;
    while(1){
	bzero(buf,100);
        ret=recv(sk, buf,sizeof(buf),0);
        if (ret == -1) {
            perror("when recv");
            ret = -1;
        }if (ret == 0) {
	    printf("recv EOF,break\n");
	    break;
	} else{
	    /*
             *printf("recv %d bytes,%s\n",ret,buf);
	     */
        }
    }

}

void childrun(int sk)
{
    dorecv(sk);
    close(sk);
}

int acceptcli(int sklis)
{
    struct pollfd skpoll;
    skpoll.fd = sklis;
    skpoll.events = POLLIN;
    skpoll.revents = 0;
    int nr=0;
    int clisk;
    struct sockaddr_in cliaddr;

    pid_t cpid;

    while(1) {
        printf("before poll\n");
        do {
            nr = poll(&skpoll,1,-1);
        }while(nr <= 0);

        socklen_t cliaddr_len = sizeof(cliaddr);
        if(skpoll.events == POLLIN) {
            clisk = accept(sklis,(struct sockaddr*) &cliaddr,&cliaddr_len);
	    printf("accpet\n");
	    printf("	cliaddr:%s(%#x):%d\n",inet_ntoa(cliaddr.sin_addr),ntohl(cliaddr.sin_addr.s_addr),ntohs(cliaddr.sin_port));

	    cpid = fork();
	    if (cpid == 0){
		childrun(clisk);
	    } else if (cpid) {
		//parent,do none
	    } else {
		printf("fork error exit\n");
		exit(-1);
	    }
        }
    }
    return 0;
}

//man 7 ip for more info
int main(void)
{
    int sklisten;
    int ret;
    struct sockaddr_in svraddr={0};
    unsigned port_listen = 8080;

    sklisten = socket(AF_INET,SOCK_STREAM,0);

    svraddr.sin_family = AF_INET;
    svraddr.sin_port = htons(port_listen);
    svraddr.sin_addr.s_addr = INADDR_ANY;
    /*
     *if(inet_aton("192.168.137.100",&svraddr.sin_addr) == 0){
     *    perror("when valued addr");
     *    ret = -11;
     *    goto out;
     *}
     */
    if( bind(sklisten,(struct sockaddr*)&svraddr,sizeof(svraddr)) ) {
        perror("when bind");
        ret = -1;
        goto out;
    }

    printf("before listen\n");
    if( listen(sklisten,300) ) {
        perror("when listen");
        ret = -2;
        goto out;
    }

    acceptcli(sklisten);

out:
    close(sklisten);

    return ret;
}

