#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/poll.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{

    int sk;
    int ret;
    struct sockaddr_in cli_addr={0};
    struct sockaddr_in svr_addr={0};

    sk = socket(AF_INET,SOCK_STREAM,0);

    int one = 1;
    setsockopt(sk, SOL_SOCKET , TCP_NODELAY , (char *)&one, sizeof(one));
    /*
     *cli_addr.sin_family = AF_INET;
     *cli_addr.sin_port = htons(4040);
     */

    /*
     *inet_aton("192.168.137.101",&cli_addr.sin_addr);
     */

    if(bind(sk,(struct sockaddr*)&cli_addr,sizeof(cli_addr))){
        perror("when bind");
        ret = -1;
        goto out;
    }

    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(8080);
    inet_aton("127.0.0.1",&svr_addr.sin_addr);
    printf("before connect\n");
    if(connect(sk,(struct sockaddr*) &svr_addr,sizeof(svr_addr))){
        printf("connect error: %s,return\n",strerror(errno));
        ret = -2;
        goto out;
    }

    char tosend=' ';
    do {
	tosend = random()%(127-33) + 33;
	ret=send(sk,&tosend,sizeof(char),0);
	if(ret == -1){
	    perror("when send");
	    ret = -3;
	    goto out;
	}else{
	    printf("send:%c\n",tosend);
	}

	/*
	 *usleep(10);
	 */
    } while(1);

    ret = 0;
out:
    close(sk);
    return ret;
}

