.PHONY:all
all:
	@gcc -o client client.c -lssl  -lcrypto
	@gcc -o service service.c -lssl -lcrypto

clean:
	@rm client service

