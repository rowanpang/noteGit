#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{

    int sk;
    int ret;
    char buf[100];
    struct sockaddr_in cliAddr={0};
    struct sockaddr_in srvrAddr={0};

    sk = socket(AF_INET,SOCK_STREAM,0);
    /*cliAddr.sin_family = AF_INET;*/
    /*cliAddr.sin_port = htons(4040);*/
    /*inet_aton("192.168.137.101",&cliAddr.sin_addr);*/
     /*if(bind(sk,(struct sockaddr*)&cliAddr,sizeof(cliAddr))){*/
        /*perror("when bind");*/
        /*ret = -1;*/
        /*goto out;*/
    /*}*/

    srvrAddr.sin_family = AF_INET;
    srvrAddr.sin_port = htons(1066);
    inet_aton("127.0.0.1",&srvrAddr.sin_addr);
    printf("before connect\n");    
    if(connect(sk,(struct sockaddr*) &srvrAddr,sizeof(srvrAddr))){
        printf("connect error: %s,return\n",strerror(errno));    
        ret = -2;
        goto out;
    }
    
    bzero(buf,100);
    while(1){
	strcpy(buf,"from client-->service");
	ret=send(sk,buf,strlen(buf),0);
	if(ret == -1){
	    perror("when send");
	    ret = -3;
	    goto out;
	}else{
	    printf("total %d,send %d bytes\n",strlen(buf),ret);
	}

	bzero(buf,100);
	struct pollfd fds[1]={
	    {
		.fd = sk,
		.events = POLLIN,
	    },
        };

	ret = poll(fds,sizeof(fds)/sizeof(struct pollfd),-1);
	printf("poll return :%d\n",ret);
	printf("poll revents :%d\n",fds[0].revents);

	if (fds[0].revents & POLLIN){
	    printf("client can read\n");
	}

	ret=recv(sk, &buf, sizeof(buf), 0);
	if(ret == -1){
	    perror("when recv");
	    ret = -4;
	    goto out;
	}else{
	    printf("cli recv %d bytes,%s\n",ret,buf); 
	}
    }

    ret = 0;
out:
    close(sk);
    return ret;
}

