#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<sys/poll.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>

void Perror(const char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

int main()
{
    int sockfd;
    short dport = 9527; //dport
    short sport = 31000; //dport
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;

    servaddr.sin_family = AF_INET; //IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(dport);

    cliaddr.sin_family = AF_INET; //IPv4
    inet_aton("127.0.0.1",&cliaddr.sin_addr);
    cliaddr.sin_port = htons(sport);

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //TCP
    int flag = 1;
    if (-1 == setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag))) {
        Perror("setsockopt fail");
    }

    int res = bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(servaddr));
    if (0 == res)
        printf("server bind success, 0.0.0.0:%d\n", dport);
    else {
        Perror("bind fail");
    }


    sleep(5);

    return 0;
}
