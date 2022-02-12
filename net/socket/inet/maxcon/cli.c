#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#define ERR_EXIT(m)\
    do{\
        perror(m); \
    }while(0)

int main(int argc,char **argv)
{
    int socketfd, connfd;
    long i = 0;
    while(1) {
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0)
		ERR_EXIT("SOCKET");
	printf("socketfd = %d,", socketfd);
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(6888);

	//无限循环建立连接，不做数据处理
	/*
	int opt = 1;
	int ret2 = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if(ret2 < 0)
	    ERR_EXIT("setsockopt");
	*/
	printf("%ld\n",i++);
	socklen_t socklen = sizeof(serveraddr);
	connfd = connect(socketfd, (const struct sockaddr*)&serveraddr, socklen);
	if(connfd < 0)
	    ERR_EXIT("connfd");
	close(socketfd);
	//usleep(10);
    }
}
