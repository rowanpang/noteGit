#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<stdlib.h>
#include<sys/poll.h>
#include<signal.h>
#include<openssl/ssl.h>
#include<openssl/err.h>

int ctrl_c=0;
void ctrl_c_handler(int signum){
	printf("handler for ctrl-c\n");
	ctrl_c=1;
}

int pem_passwd_cb(char *buf,int size,int rwflag,void *passwd)
{
	strncpy(buf,(char*)passwd,size);
	buf[size-1] = 0;
	return strlen(buf);
}

int main(int argc,char** argv)
{
	int sk_listen;
	int ret;
	struct sockaddr_un svr_addr={0};
	SSL_CTX *sctx;

	if(argc < 3){
		printf("usage:%s pub pri\n",argv[0]);
		exit(-20);	
	}
	signal(SIGINT,ctrl_c_handler);

	SSL_library_init();
	SSL_load_error_strings();
	sctx=SSL_CTX_new(SSLv23_server_method());
	
	/* hotkey ',cs'
	 *加密形式的私钥需要密码解密，callback提供密码.
	 *如果没有提供cb，则将从stdin read，prompt input passwd.
	 *增加cb，不需要密码的私钥作为参数，不受影响.
	 */
	SSL_CTX_set_default_passwd_cb_userdata(sctx,"Inspur_pangwz");
	SSL_CTX_set_default_passwd_cb(sctx,pem_passwd_cb);

	SSL_CTX_use_certificate_file(sctx,argv[1],SSL_FILETYPE_PEM);
	SSL_CTX_use_PrivateKey_file(sctx,argv[2],SSL_FILETYPE_PEM);
	if(SSL_CTX_check_private_key(sctx) == 1){
		printf("private key check ok\n");
	}else{
		ERR_print_errors_fp(stdout);
		errno = -1;
		perror("when ssl ctx chek private key");
		ret = -10;
		exit(ret);
	}

	
	sk_listen = socket(AF_UNIX,SOCK_STREAM,0);
	svr_addr.sun_family = AF_UNIX;
	strcpy(svr_addr.sun_path,"./svr_socket");
	
	if(bind(sk_listen,(struct sockaddr*)&svr_addr,sizeof(svr_addr))){
		perror("when bind");
		ret = -1;
		goto out;
	}
	
	printf("before listen\n");	
	if(listen(sk_listen,8)){
		perror("when listen");
		ret = -2;
		goto out;
	}

	struct pollfd svr_poll;
	svr_poll.fd = sk_listen;
	svr_poll.events = POLLIN;
	svr_poll.revents = 0;
	
	while(1){
		printf("before poll\n");	
		SSL *ssl;
		int nr=0;
		do{
			nr = poll(&svr_poll,1,100);
			if(ctrl_c){
				printf("terminal by ctrl-c\n");
				ret = 0;
				goto out;
			}
		}while(nr <= 0);

		int sk;
		struct sockaddr_un cli_addr;
		socklen_t cli_addr_size = sizeof(cli_addr);
		if(svr_poll.events == POLLIN){
			sk = accept(sk_listen,(struct sockaddr*) &cli_addr,&cli_addr_size);
			printf("cli_addr:%s\n",cli_addr.sun_path);
		}	

		ssl = SSL_new(sctx);
		SSL_set_fd(ssl,sk);
	
		if(SSL_accept(ssl) != 1){
			ERR_print_errors_fp(stdout);
			errno = -1;
			perror("when ssl_accept");
			ret = -10;
			goto next;
		}
		char buf[100] = {0};
		/*ret=recv(sk, buf, sizeof(buf), 0);	*/
		ret = SSL_read(ssl,buf,99);
		if(ret > 0){
			printf("recv %d bytes,%s\n",ret,buf);
		}else{
			ERR_print_errors_fp(stdout);
			errno = -1;
			perror("when recv");
			ret = -3;
			goto next;
		}
		bzero(buf,100);
		strcpy(buf,"from service-->client");
		/*ret=send(sk,buf,strlen(buf),0);*/
		ret = SSL_write(ssl,buf,strlen(buf));
		if(ret > 0){
			printf("total %d, %d bytes send\n",strlen(buf),ret);	
		}else{
			ERR_print_errors_fp(stdout);
			errno = -1;
			perror("when send");
			ret = -4;
			goto next;
		}
	next:
		SSL_shutdown(ssl);
		SSL_free(ssl);
		close(sk);
		if(!ret){
			printf("client connection failed\n");
			goto out;
		}
		/*if(ctrl_c){*/
			/*printf("terminal by ctrl-c\n");*/
			/*ret = 0;*/
			/*goto out;*/
		/*}*/
	}
	ret = 0;
out:
	SSL_CTX_free(sctx);
	close(sk_listen);
	unlink(svr_addr.sun_path);

	return ret;
}

