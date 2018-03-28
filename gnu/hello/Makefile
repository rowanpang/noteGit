.PHONY:all clean
all:hello

hello:main.c
	@gcc -g -O main.c -o $@
	@eu-strip --remove-comment -f $@.debug $@
clean:
	@-rm -rf a.out
	@-rm -rf hello
	@-rm -rf hello.debug
	@-rm *.o
