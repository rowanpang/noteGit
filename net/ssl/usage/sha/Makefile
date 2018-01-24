.PHONY:all

all:main.c
	@gcc main.c -lcrypto
	@echo "sha1sum self:"
	@./a.out main.c
	@echo "sha1sum tools"
	@sha1sum main.c
