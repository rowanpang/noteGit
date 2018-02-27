
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<stdlib.h>
#include<sys/poll.h>

int main(void)
{

	int cl = socket(AF_UNIX,SOCK_STREAM,0);
	struct sockaddr_un sr_addr;
	memset(&sr_addr,0,sizeof(sr_addr));
	sr_addr.sun_family = AF_UNIX;
	strcpy(sr_addr.sun_path,"./sr_socket");
	
	struct sockaddr_un cl_addr;
	memset(&cl_addr,0,sizeof(cl_addr));
	cl_addr.sun_family = AF_UNIX;
	strcpy(cl_addr.sun_path,"./cl_socket");

	unlink(cl_addr.sun_path);
 	bind(cl,(struct sockaddr*)&cl_addr,sizeof(cl_addr));

	printf("before connect\n");	
	connect(cl,(struct sockaddr*) &sr_addr,sizeof(sr_addr));

	send(cl,"from client\n",12,0);
	
	char buf[100]={"\0"};
	recv(cl, &buf, sizeof(buf), 0);
	printf("cl recv:%s\n",buf);
	sleep(10);
	close(cl);
	return 0;
}

