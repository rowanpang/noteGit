#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>

#include <netinet/in.h>
#include <netinet/ip.h>

#include <arpa/inet.h>


#define LISTENNUM	20
#define EPOLLSIZE	(LISTENNUM*2)
#define EVENTPERPOLL	10
#define LISTENPORT	8080
#define LINELEN		128
int totalCl=0;

int handleClient(struct epoll_event ev)
{
    int sockfd;
    int ret;
    char line[LINELEN];

    printf("handleClient:evt:%d,fd:%d\n",ev.events,ev.data.fd );

    bzero(line,sizeof(line));
    if(ev.events & EPOLLIN){
	sockfd = ev.data.fd;
	/*per read receive LINELEN,if there more data will be here again*/
	ret = read(sockfd,line,LINELEN);
	if(ret < 0){
	    if(errno == ECONNRESET){
		totalCl--;
		close(sockfd);
	    }
	}else if(ret == 0){
	    /*child exit or finished*/
	    totalCl--;
	    close(sockfd);
	}else{
	    printf("    received data len:%d,%s\n",ret, line);
	}

	bzero(line,sizeof(line));
	sprintf(line,"recv ok!");
	ret = write(sockfd,line,strlen(line));
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int ret;
    int lsenfd,clfd;
    struct sockaddr_in lsenaddr,claddr;
    int epfd,nfds;
    int i;
    socklen_t addrlen;


    struct epoll_event evtmp,evpoll[EVENTPERPOLL];

    lsenaddr.sin_family = AF_INET;
    lsenaddr.sin_port = htons(LISTENPORT);
    if(!inet_aton("127.0.0.1",&lsenaddr.sin_addr)){
	perror("when inet_aton");
	ret = -1;
	goto out;
    }

    lsenfd = socket(AF_INET,SOCK_STREAM,0);

    if(bind(lsenfd,(struct sockaddr*)&lsenaddr,sizeof(lsenaddr))){
	perror("when bind");
	ret = -2;
	goto socketError;
    }

    if(listen(lsenfd,LISTENNUM)){
	perror("when listen");
	ret = -3;
	goto socketError;
    }else{
	printf("listen fd:%d\n",lsenfd );
    }

    epfd = epoll_create(EPOLLSIZE);
    if (epfd == -1){
	perror("epoll_create");
	ret = -4;
	goto socketError;
    }else{
	printf("epoll fd:%d\n",epfd);
    }

    bzero(&evtmp,sizeof(evtmp));
    evtmp.data.fd = lsenfd;
    evtmp.events = EPOLLIN;
    if (epoll_ctl(epfd,EPOLL_CTL_ADD,lsenfd,&evtmp) == -1){
	perror("epoll_ctl add lsenfd");
	ret = -5;
	goto epollError;
    }

    while(1){
	nfds = epoll_wait(epfd,evpoll,EVENTPERPOLL,-1);
	if(nfds == -1){
	    perror("epoll_wait");
	    ret = -5;
	    goto epollError;
	}
	printf("epoll nfds:%d\n",nfds);

	for (i = 0; i < nfds; ++i) {
	    if(evpoll[i].data.fd == lsenfd){
		printf("new client:%d\n",totalCl++);
		addrlen = sizeof(claddr);
		clfd = accept(lsenfd,(struct sockaddr*) &claddr,&addrlen);
		if(clfd == -1){
		    perror("accept client");
		    ret = -6;
		    goto epollError;
		}
		printf("    cli addr# %s:%d\n",inet_ntoa(claddr.sin_addr), ntohs(claddr.sin_port));

		bzero(&evtmp,sizeof(evtmp));
		evtmp.data.fd = clfd;
		evtmp.events = EPOLLIN;
		if (epoll_ctl(epfd,EPOLL_CTL_ADD,clfd,&evtmp) == -1){
		    perror("epoll_ctl add lsenfd");
		    ret = -5;
		    goto epollError;
		}

	    }else {
		handleClient(evpoll[i]);
	    }
	}

	printf("\n");
    }


    ret = 0;
epollError:
    close(epfd);

socketError:
    close(lsenfd);

out:
    return ret;
}
