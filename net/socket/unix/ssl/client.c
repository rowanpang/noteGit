#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<stdlib.h>
#include<sys/poll.h>
#include<errno.h>
#include<string.h>
#include<openssl/ssl.h>
#include<openssl/err.h>

void ShowCerts(SSL * ssl)
{
    X509 *cert;
    char *line;
    long result;

    // 该函数可以从SSL套接字中提取对方的证书信息，这些信息已经被SSL验证过了。
    X509_NAME *X509_get_subject_name(X509 *a);
    // 该函数得到证书所用者的名字。返回值是X509
    cert = SSL_get_peer_certificate(ssl);

    if (cert != NULL) {
        printf("数字证书信息:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("证书: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("颁发者: %s\n", line);
        result = SSL_get_verify_result(ssl);
        printf("校验结果: %ld\n", result);

        free(line);
        X509_free(cert);
    } else{
        printf("无证书信息！\n");
    }
}


int main(int argc,char** argv)
{

    int sk;
    int ret;
    struct sockaddr_un cli_addr={0};
    struct sockaddr_un svr_addr={0};

    SSL_CTX *sctx;
    SSL *ssl;

    SSL_library_init();
    sctx = SSL_CTX_new(SSLv23_client_method());    

    /*导入可信任的证书，用来verify server的数字证书.*/
    printf("ca file:%s\n",argv[1]);
    if(SSL_CTX_load_verify_locations(sctx,argv[1],NULL) == 0){
        perror("when load verify CA");
        ERR_print_errors_fp(stdout);
        ret = -1;
        /*goto out3;*/
    }

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
    
    ssl = SSL_new(sctx);
    SSL_set_fd(ssl,sk);
    if(SSL_connect(ssl) == 1){
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        ShowCerts(ssl);
    }else{
        ERR_print_errors_fp(stdout);
        errno=-1;
        perror("when ssl_connect");
        ret = -21;
        goto out;
    }


    char buf[100]={0};
    bzero(buf,100);
    strcpy(buf,"from client-->service");
    /*ret=send(sk,buf,strlen(buf),0);*/
    ret = SSL_write(ssl,buf,strlen(buf));
    if(ret > 0){
        printf("total %d,send %d bytes\n",strlen(buf),ret);
    }else{
        ERR_print_errors_fp(stdout);
        errno=-1;
        perror("when send");
        ret = -3;
        goto out;
    }

    bzero(buf,100);
    /*ret=recv(sk, &buf, sizeof(buf), 0);*/
    ret = SSL_read(ssl,buf,99);
    if(ret > 0){
        printf("cli recv %d bytes,%s\n",ret,buf); 
    }else{
        ERR_print_errors_fp(stdout);
        errno=-1;
        perror("when recv");
        ret = -4;
        goto out;
    }

    ret = 0;
out:
    SSL_free(ssl);
    close(sk);
    unlink(cli_addr.sun_path);
out3:
    SSL_CTX_free(sctx);
    return ret;
}

