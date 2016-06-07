#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<stdlib.h>
#include<sys/poll.h>
#include<errno.h>
#include<string.h>

int main(void)
{

	int sk;
	int ret;
	struct sockaddr_un cli_addr={0};
	struct sockaddr_un svr_addr={0};

	sk = socket(AF_UNIX,SOCK_STREAM,0);
	svr_addr.sun_family = AF_UNIX;
	strcpy(svr_addr.sun_path,"./svr_socket");

	cli_addr.sun_family = AF_UNIX;
	strcpy(cli_addr.sun_path,"./cli_socket");

 	if(bind(sk,(struct sockaddr*)&cli_addr,sizeof(cli_addr))){
		perror("when bind");
		ret = -1;
		goto out;
	}
	
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
	unlink(cli_addr.sun_path);
	return ret;
}

