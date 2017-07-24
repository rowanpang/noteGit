#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <poll.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define  CHK 3

#if CHK == 1
    #define COMPILE_OK  1
#elif CHK == 2
    #define COMPILE_NG  1
#elif CHK == 3
    #define RUNTIME_CHECK_ABLE 1
#elif CHK == 4
    #define RUNTIME_CHECK_ABLE_NG 1
#endif 

int constantArgTest(int inNum);

inline int constantArgTest(int inNum)
{
    printf("__builtin_constant_p(inNum) :%s\n", __builtin_constant_p(inNum)? "yes" : "no" );

    return inNum;
}
int myPoll2(struct pollfd ufds[], unsigned int nfds, int timeout_ms) 
{
    printf("__bos(ufds):%d\n",__bos(ufds));
    printf("__builtin_constant_p(nfds) :%s\n", __builtin_constant_p(nfds)? "yes" : "no" ); //when compile, can not detimine the size of ufds. so can not fortify.
    poll(ufds,nfds,timeout_ms);

    return 0;
}

/*extern int __poll_chk (struct pollfd *__fds, nfds_t __nfds, int __timeout, __SIZE_TYPE__ __fdslen);*/
/*__poll_chk(pfd,num,2,__bos(pfd));*/		    //direct call

int myPoll(int readfd0,int readfd1,int writefd,int inNum)
{
    struct pollfd pfd[3];
    int num;
    int ret;
    printf("__bos(pfd):%d\n",__bos(pfd));
    constantArgTest(5);

#if defined COMPILE_OK
    num = 0;
    pfd[num].fd = readfd0;
    pfd[num].events = POLLRDNORM|POLLIN|POLLRDBAND|POLLPRI;
    pfd[num].revents = 0;
    num++;
    ret = poll(pfd, num, 0);

#elif defined COMPILE_NG
    num = 0;
    pfd[num].fd = readfd0;
    pfd[num].events = POLLRDNORM|POLLIN|POLLRDBAND|POLLPRI;
    pfd[num].revents = 0;
    num++;
    num += 4;
    ret = poll(pfd, num, 0);

#elif defined RUNTIME_CHECK_ABLE
    num = 0;
    if(readfd0 != -1) {
	pfd[num].fd = readfd0;
	pfd[num].events = POLLRDNORM|POLLIN|POLLRDBAND|POLLPRI;
	pfd[num].revents = 0;
	num++;
    }
    if(readfd1 != -1) {
	pfd[num].fd = readfd1;
	pfd[num].events = POLLRDNORM|POLLIN|POLLRDBAND|POLLPRI;
	pfd[num].revents = 0;
	num++;
    }
    if(writefd != -1) {
	pfd[num].fd = writefd;
	pfd[num].events = POLLWRNORM|POLLOUT;
	pfd[num].revents = 0;
	num++;
    }
    num += 4; /*force error*/
    ret = poll(pfd, num, 0);

#elif defined RUNTIME_CHECK_ABLE_NG 
    pfd[num].fd = writefd;
    pfd[num].events = POLLWRNORM|POLLOUT;
    pfd[num].revents = 0;
    num++;
    if(writefd != -1) {
	num++;
    }

    ret = myPoll2(pfd,num,0);
#endif

    return ret;
}


int main(int argc, char *argv[])
{
    int ret;
    int skfd;
    int num;
    struct sockaddr_in srvrAddr;
    printf("pid: %d\n",getpid());
    printf("__bos(NULL):%d\n",__bos(NULL));		     /*__bos: 如果可以在编译时确认size则返回size,不能则返回-1;*/
    printf("__bos(0x7fffffffd830):%d\n",__bos((struct poofd*)0x7fffffffd830));
    printf("____USE_FORTIFY_LEVEL:%d\n",__USE_FORTIFY_LEVEL);

#ifdef __fortify_function
    printf("__fortify_function defined\n");
#else
    printf("__fortify_function not defined\n");
#endif

    //ref:https://mudongliang.github.io/2016/05/25/fortify_source-examples.html
    /*
     *char buffer[10];
     *bzero(buffer,10);
     *strcpy(buffer, "abcde");
     *strcpy(buffer, "abcdefghijklmn");	    //explict error
     *strcpy(buffer, argv[1]);		    //error can detect when running.
     *printf("buffer: %s\n",buffer);
     */
    skfd = socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK,0);
    srvrAddr.sin_family = AF_INET;
    srvrAddr.sin_port = htons(1066);
    inet_aton("127.0.0.1",&srvrAddr.sin_addr);

    if(connect(skfd,(struct sockaddr*) &srvrAddr,sizeof(srvrAddr))){
	perror("when connect");
	if(errno != EINPROGRESS){
	    ret = -1;
	    goto out;
	}	
    }
    ret = myPoll(-1,-1,skfd,4);

    //sleep(1);
    ret = 0;
out:

    return 0;
}
