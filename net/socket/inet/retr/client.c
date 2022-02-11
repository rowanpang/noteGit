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

    int skfd;
    int ret;
    struct sockaddr_in svr_addr={0};
    int one = 1;

    skfd = socket(AF_INET,SOCK_STREAM,0);


    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(31337);
    inet_aton("10.152.6.219",&svr_addr.sin_addr);

    printf("before connect\n");
    if(connect(skfd,(struct sockaddr*) &svr_addr,sizeof(svr_addr))){
        printf("connect error: %s,return\n",strerror(errno));
        ret = -2;
        goto out;
    }

    #define READ_SIZE 4
    char buf[READ_SIZE];
    do {
        printf("input char to send, no to stop\n");
        ret = read(STDIN_FILENO,buf,READ_SIZE);
        if(ret < 0 || !strncmp("no",buf,2)){
	    ret = -1;
	    perror("when read");
            goto out;
        }
        char c;
        printf("ret %d\n",ret);
        if((ret == READ_SIZE) && (buf[READ_SIZE - 1] != '\n')) {         //buf full,need flush stdin
            printf("flush\n");
            while ((c=getchar()) != '\n' && c != EOF);
        }
	ret=send(skfd,buf,ret,0);
	if(ret == -1){
	    perror("when send");
	    ret = -3;
	    goto out;
	}
        //usleep(10);
    } while(1);

    ret = 0;
out:
    close(skfd);
    return ret;
}

