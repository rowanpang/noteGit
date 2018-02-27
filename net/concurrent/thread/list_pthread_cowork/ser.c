#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<stdlib.h>
#include<sys/poll.h>
#include<pthread.h>
#include<errno.h>

#define true 1
#define TRUE true
#define Yes true

#define false 0
#define FALSE false
#define No	false

#ifndef NULL
#define NULL 0
#endif

#define NOUSE(x)	(x)=(x)
typedef struct client{
	struct client* next;
	int fd_sk;
	int isConnected;
	pthread_t th;
	void (*handler_pre)(struct client* cli);
	void (*handler_post)(struct client* cli);
}client;

typedef struct wclient{
	client* head;
	int num;
	pthread_mutex_t lock;
}wclient;

static wclient* wholeClient=NULL;

void wclient_init()
{
	wholeClient = malloc(sizeof(wclient));
	wholeClient->head = NULL;
	wholeClient->num = 0;
	pthread_mutex_init(&wholeClient->lock,NULL);
}	

void wclient_destroy()
{
	pthread_mutex_destroy(&wholeClient->lock);
	printf("all release %s\n",wholeClient->num?"NO":"YES");
	free(wholeClient);
}
//private do not directly call;
void client_add_tail(client* newcl)
{
	printf("%s---in\n",__FUNCTION__);
	client* tmp = wholeClient->head;
	static int i = 0;
	if(tmp == NULL)
	{
		i++;
		printf("init here,%d\n",i);
		wholeClient->head = newcl;
		newcl->next = wholeClient->head;
		goto OUT;
	}
	while(tmp->next != wholeClient->head) 
   	{
		i++;
		printf("while find,already had:%d\n",i);
		tmp = tmp->next;
	}
	newcl->next = wholeClient->head;
	tmp->next = newcl;
OUT:
	printf("%s---out\n",__FUNCTION__);
	return;
}
//private do not directly call;
void client_remove(client* newcl)
{
	client* tmp = wholeClient->head;
	while(tmp&&(tmp->next != newcl))
   	{
		tmp = tmp->next;
	}
	if(tmp->next == wholeClient->head)
	{
		printf("not found");
	}
	else
	{
		tmp->next = newcl->next;	
	}	
	return;
}

void client_handler_pre(client* cli)
{
	NOUSE(cli);
	return;
}

void client_handler_post(client* cli)
{
	NOUSE(cli);
	return;
}

void client_init(client*cli,int fd_sk)
{
	cli->next=NULL;
	cli->fd_sk = fd_sk;
	cli->isConnected = Yes;	
	cli->handler_pre = client_handler_pre;
	cli->handler_post = client_handler_post;
}

void client_add(client* cli)
{
	pthread_mutex_lock(&wholeClient->lock);
	client_add_tail(cli);
	wholeClient->num++;	
	printf("%s,cur num:%d\n",__FUNCTION__,wholeClient->num);
	pthread_mutex_unlock(&wholeClient->lock);
}

void client_del()
{
	printf("%s---in\n",__FUNCTION__);
	pthread_mutex_lock(&wholeClient->lock);
	if(wholeClient->head == NULL)
	{
		printf("head equal null error!!!\n");
		return;
	}
	client* tmp = wholeClient->head->next;
	client* tmp_2 = NULL;
	while(tmp && (tmp != wholeClient->head))
	{
		printf("join client thread,isConnected:%s\n",tmp->isConnected?"Yes":"NO");
		pthread_join(tmp->th,NULL);
		tmp_2 = tmp->next;
		free(tmp);
		wholeClient->num--;	
		tmp = tmp_2;
	}
	printf("com here release head\n");
	pthread_join(tmp->th,NULL);
	free(tmp);	
	wholeClient->num--;	
	tmp = NULL;
	pthread_mutex_unlock(&wholeClient->lock);
	printf("%s---out\n",__FUNCTION__);
}

void *client_handler(void* sk_cli)
{
	client* sk_client = (client*)sk_cli; 
	sk_client->handler_pre(sk_client);

	char buf[100] = {"hello"};
	recv(sk_client->fd_sk,&buf,sizeof(buf), 0);	
	printf("%s,recv:%s\n",__FUNCTION__,buf);

	send(sk_client->fd_sk,"server ask",11,0);
	sleep(10);

	sk_client->handler_post(sk_client);
	printf("client out,disconnect \n");
	sk_client->isConnected = No;
	close(sk_client->fd_sk);
}
int accept_out()
{
	int ret = 0;
	printf("%s---in\n",__FUNCTION__);
	pthread_mutex_lock(&wholeClient->lock);
	if(wholeClient->num == 5)
	{
		ret = true;
	}
	pthread_mutex_unlock(&wholeClient->lock);
	printf("%s---out\n",__FUNCTION__);
	return ret;
}
void *accept_handler(void *sk_lis)
{
	int *sk_listen = (int*)sk_lis; 
	struct pollfd sr_poll;
	sr_poll.fd = *sk_listen;
	sr_poll.events = POLLIN;
	sr_poll.revents = 0;
	int nr=0;
	int cli_sk;
	struct sockaddr_un cli_addr;	
	while(1)
	{
		printf("new loop\n");	
		do
		{
			nr = poll(&sr_poll,1,-1);
		}while(nr <= 0);

		socklen_t cli_addr_len = sizeof(cli_addr);
		if(sr_poll.events == POLLIN)
		{
			cli_sk = accept(*sk_listen,(struct sockaddr*) &cli_addr,&cli_addr_len);
			printf("accept cli_addr:%s\n",cli_addr.sun_path);
		}	
		printf("accept new client\n");
		client* cli = malloc(sizeof(client)); 	
		client_init(cli,cli_sk);
		pthread_create(&cli->th,NULL,&client_handler,cli);
		client_add(cli);
		if(accept_out())
			break;
	}
	client_del();
	return 0;
}

int main(void)
{
	wclient_init();
	int sr_ls = socket(AF_UNIX,SOCK_STREAM,0);
	struct sockaddr_un sr_addr;
	memset(&sr_addr,0,sizeof(sr_addr));
	sr_addr.sun_family = AF_UNIX;
	//sr_addr.sun_path[108] = {"sr_socket"};	//error,just can be valued when init,like below 
	strcpy(sr_addr.sun_path,"./sr_socket");
	
	int ret = unlink(sr_addr.sun_path);
	if (ret != 0 && errno != ENOENT) {
		printf("Failed to unlink old socket'%s'\n", strerror(errno));
		goto out_close;
	}
	bind(sr_ls,(struct sockaddr*)&sr_addr,sizeof(sr_addr));
	
	printf("before listen\n");	
	listen(sr_ls,8);

	pthread_t accept_th;
	pthread_create(&accept_th,NULL,&accept_handler,&sr_ls);
	pthread_join(accept_th,NULL);

	printf("finish accept\n");

out_close:	
	wclient_destroy();
	close(sr_ls);
	return 0;
}

