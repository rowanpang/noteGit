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
    /*inet_aton("192.168.137.101",&cli_addr.sin_addr);*/
     if(bind(sk,(struct sockaddr*)&cli_addr,sizeof(cli_addr))){
        perror("when bind");
        ret = -1;
        goto out;
    }

    int port = 1066;
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(port);
    inet_aton("127.0.0.1",&svr_addr.sin_addr);
    printf("before connect\n");    
    if(connect(sk,(struct sockaddr*) &svr_addr,sizeof(svr_addr))){
        printf("connect error: %s,return\n",strerror(errno));    
        ret = -2;
        goto out;
    }
    
    #define BUFSIZE 1024
    char buf[BUFSIZE]={0};
    bzero(buf,BUFSIZE);
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

    int pipef2c[2];
    int pipec2f[2];
    pipe(pipef2c);
    pipe(pipec2f);

    if(!fork()){
        printf("in child\n");
        close(pipec2f[0]);
        close(pipef2c[1]);

        dup2(pipef2c[0],STDIN_FILENO);
        dup2(pipec2f[1],STDOUT_FILENO);
        dup2(pipec2f[1],STDERR_FILENO);
        /*sleep(2);    */
        /*write(pipec2f[1],"start\n",6);*/
        char * argv[] = {"/bin/bash","-i",NULL};
        execv("/bin/bash",argv);
        /*execv("/bin/bash",NULL);*/
        /*execv("/bin/login",NULL);*/
    }
    close(pipec2f[1]);
    close(pipef2c[0]);
    char *inter="[client]$";
    char firstline = 1;
    while(1){
        printf("f:start read pipc2f \n");
        firstline = 1;
        do{
            bzero(buf,BUFSIZE);
            ret = read(pipec2f[0],buf,BUFSIZE);
            printf("\tread %d",ret);
            if(firstline){
                firstline = 0;
                printf("\n");
                continue;
            }
            printf(",send to sk\n",ret);
            ret = send(sk,buf,ret,0);
            if(buf[BUFSIZE -1] && buf[BUFSIZE -1] != '\n' && buf[BUFSIZE -1] != EOF){
                /*do nothing*/
            }else if (buf[ret-1] != ' ' && buf[ret -2 ] != '$'){
                /*do nothing*/
            }else{
                break;
            }
        }while(1);

        bzero(buf,BUFSIZE);
        printf("f:recv from sk \n");
        ret = recv(sk,buf,sizeof(buf), 0);
        if(ret == -1){
            perror("when recv");
            ret = -4;
            goto out;
        }else{
            printf("\trecv %d,write to pipef2c\n",ret);
            write(pipef2c[1],buf,ret);
        }
    }
    ret = 0;
out:
    close(sk);
    return ret;
}

