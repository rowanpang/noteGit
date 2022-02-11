.PHONY:all
all:
	@gcc -o client client.c -lssl -lcrypto -Wl,-rpath=/home/pangwz/rpmbuild/BUILD/openssl-1.0.2k 
	@gcc -o service service.c -lssl -lcrypto -Wl,-rpath=/home/pangwz/rpmbuild/BUILD/openssl-1.0.2k 

clean:
	@rm client service

