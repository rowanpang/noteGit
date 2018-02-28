#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define SVRPORT	8080
#define SVRIP	"127.0.0.1"
int main(int argc, char *argv[])
{
    int ret;
    int *pclients;
    int clientNum = 130;
    int i;
    int clentfd;
    struct sockaddr_in svraddr;
    char msg[128];

    pclients = malloc(clientNum * sizeof(int));

    svraddr.sin_family = AF_INET;
    svraddr.sin_port = htons(SVRPORT);
    if(!inet_aton(SVRIP,&svraddr.sin_addr)){
	perror("when inet_aton");
	ret = -1;
	goto out;
    }

    for (i = 0; i < clientNum; ++i) {
	clentfd = socket(AF_INET,SOCK_STREAM,0);
	pclients[i] = clentfd;
	ret = connect(clentfd,(struct sockaddr*)&svraddr,sizeof(svraddr));
	if(ret == -1){
	    perror("connect");
	    goto sockError;
	}

	bzero(msg,128);
	sprintf(msg,"clent:%d,hello server",i);
	ret = write(clentfd,msg,strlen(msg));

	bzero(msg,128);
	ret = read(clentfd,msg,128);
	printf("cli:%d receive msg:%s\n",i,msg);
    }

    sleep(300);
    ret = 0;
sockError:
    for (; i >= 0; i--) {
	close(pclients[i]);
    }

out:
    free(pclients);
    return ret;
}
