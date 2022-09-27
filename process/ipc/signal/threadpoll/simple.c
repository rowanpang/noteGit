#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/poll.h>
#include <sys/syscall.h>


#define gettid() syscall(__NR_gettid)

void* threadfn(void *arg)
{
    printf("in thread:%d\n",gettid());

    int sk;
    int ret;
    char buf[100];
    struct sockaddr_in cliAddr={0};
    struct sockaddr_in srvrAddr={0};

    sk = socket(AF_INET,SOCK_STREAM,0);

    srvrAddr.sin_family = AF_INET;
    srvrAddr.sin_port = htons(1066);
    inet_aton("127.0.0.1",&srvrAddr.sin_addr);
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
    return NULL;
}

int main(int argc,char** argv)
{
    int ret;
    int *status = &ret;
    pthread_t th;
    printf("in main:%d\n",getpid());

    ret = pthread_create(&th,NULL,threadfn,NULL);
    if(ret){
	perror("error when pthread create");
	goto ERROR_1;
    }

    /*sleep(30);*/
    pthread_join(th,NULL);
    printf("thread finished ret:%d\n",ret);
    ret = 0;
ERROR_1:
    return ret;
}
