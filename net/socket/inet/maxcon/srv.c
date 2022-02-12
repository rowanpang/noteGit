#include <sys/types.h>       /* basic system data types */
#include <sys/socket.h>      /* basic socket definitions */
#include <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>       /* inet(3) functions */
#include <sys/epoll.h> /* epoll function */
#include <fcntl.h>     /* nonblocking */
#include <sys/resource.h> /*setrlimit */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAXEPOLLSIZE 65535
#define MAXLINE 10
int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) {
        return -1;
    }
    return 0;
}

static int iCount = 0;
static int iFaild = 0;
int main(void)
{
    int servPort = 6888;
    int listenq = 65535;

    int listenfd, connfd, kdpfd, nfds, n, curfds, acceptCount = 0;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t socklen = sizeof(struct sockaddr_in);
    struct epoll_event ev;
    struct epoll_event events[MAXEPOLLSIZE];
    struct rlimit rt;
    char buf[MAXLINE];

    /* 设置每个进程允许打开的最大文件数 */
    rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1){
        perror("setrlimit error");
        return -1;
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servaddr.sin_port = htons (servPort);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("can't create socket file");
        return -1;
    }

    int opt = 1;
    //setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (setnonblocking(listenfd) < 0) {
        perror("setnonblock error");
    }

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(struct sockaddr)) == -1){
        perror("bind error");
        return -1;
    }
    if (listen(listenfd, listenq) == -1){
        perror("listen error");
        return -1;
    }
    /* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */
    kdpfd = epoll_create(MAXEPOLLSIZE);
    //ev.events = EPOLLIN | EPOLLET;
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listenfd, &ev) < 0){
    	fprintf(stderr, "epoll set insertion error: fd=%d\n", listenfd);
    	return -1;
    }

    curfds = 1;
    printf("epollserver startup,port %d, max connection is %d, backlog is %d\n", servPort, MAXEPOLLSIZE, listenq);
    for (;;) {
    	/* 等待有事件发生 */
    	nfds = epoll_wait(kdpfd, events, curfds, -1);
    	if (nfds == -1) {
    		perror("epoll_wait");
		continue;
        }
  	printf("epoll_wait return\n");

	/* 处理所有事件 */
	for (n = 0; n < nfds; ++n){
		if (events[n].data.fd == listenfd){
			connfd = accept(listenfd, (struct sockaddr *)&cliaddr,&socklen);
			if (connfd < 0){
				iFaild ++;
				if(iFaild >= 10)
					close(listenfd);
				perror("accept error");
				continue;
			}
			iCount++;
			sprintf(buf, "accept form %s:%d ", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
			printf("%d:%s %d\n", ++acceptCount, buf, iCount);
                        recv(connfd,buf,1,0);
			close(connfd);
			continue;
		}
	}
    }
    close(listenfd);
    return 0;
}
