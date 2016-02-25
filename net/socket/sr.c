
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<stdlib.h>
#include<sys/poll.h>

int main(void)
{

	int sr_ls = socket(AF_UNIX,SOCK_STREAM,0);
	struct sockaddr_un sr_addr;
	memset(&sr_addr,0,sizeof(sr_addr));
	sr_addr.sun_family = AF_UNIX;
	//sr_addr.sun_path[108] = {"sr_socket"};	//error,just can be valued when init,like below 
	strcpy(sr_addr.sun_path,"./sr_socket");
	
	unlink(sr_addr.sun_path);
	bind(sr_ls,(struct sockaddr*)&sr_addr,sizeof(sr_addr));
	
	printf("before listen\n");	
	listen(sr_ls,8);

	struct pollfd sr_poll;
	sr_poll.fd = sr_ls;
	sr_poll.events = POLLIN;
	sr_poll.revents = 0;

	printf("before poll\n");	
	int nr=0;
	do
	{
		nr = poll(&sr_poll,1,-1);
	}while(nr <= 0);


	int sr;
	struct sockaddr_un cl_addr;
	socklen_t cl_addr_size = sizeof(cl_addr);
	if(sr_poll.events == POLLIN)
	{
		sr = accept(sr_ls,(struct sockaddr*) &cl_addr,&cl_addr_size);
		printf("cl_addr:%s\n",cl_addr.sun_path);
	}	

	char buf[100] = {"hello"};
	recv(sr, &buf, sizeof(buf), 0);	
	printf("recv:%s\n",buf);

	send(sr,"server ask",11,0);
	
	close(sr);
	close(sr_ls);
	return 0;
}

