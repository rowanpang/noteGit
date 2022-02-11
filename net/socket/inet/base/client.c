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
    struct sockaddr_in cli_addr={0};
    struct sockaddr_in svr_addr={0};

    sk = socket(AF_INET,SOCK_STREAM,0);
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(4040);
    inet_aton("127.0.0.1",&cli_addr.sin_addr);
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

    char buf[100]={0};
    bzero(buf,100);
    strcpy(buf,"from client-->service");
    ret=send(sk,buf,strlen(buf),0);
    if(ret == -1){
        perror("when send");
        ret = -3;
        goto out;
    }else{
        printf("total %d,send %d bytes\n",strlen(buf),ret);
    }
    do{
        printf("start recv (yes/no)[yes]:\n");
        #define READ_SIZE 4
        ret = read(STDIN_FILENO,buf,READ_SIZE);
        if(ret != -1){
            if(!strncmp("no",buf,2)){
                /*do nothing*/
            }else if(!strncmp("yes",buf,3)){
                break;
            }else if(!strncmp("\n",buf,1)){
                break;
            }
        }
        char c;
        printf("ret %d\n",ret);
        if((ret == READ_SIZE) && (buf[READ_SIZE - 1] != '\n')){
            printf("flush\n");
            while ((c=getchar()) != '\n' && c != EOF);
        }
    } while(1);

    bzero(buf,100);
    ret=recv(sk, &buf, sizeof(buf), 0);
    if(ret == -1){
        perror("when recv");
        ret = -4;
        goto out;
    }else{
        printf("cli recv %d bytes,%s\n",ret,buf);
    }

    ret = 0;
out:
    close(sk);
    return ret;
}

