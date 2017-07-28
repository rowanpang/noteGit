1. 编译方式:
gcc -o server server.c -Wall -g -lssl
gcc -o client client.c -Wall -g -lssl

2. 生产私钥和证书
openssl genrsa -out privkey.pem 1024
openssl req -new -x509 -key privkey.pem -out CAcert.pem -days 1095

3. 程序运行方式:
./server 7838 1 CAcert.pem privkey.pem
./client 127.0.0.1 7838

4,enable debug for openssl
    a,include/openssl/kssl.h
    b,#define         KSSL_DEBUG 
