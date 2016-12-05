.PHONY:all clean hello
all:hello gcc-hello

hello:hello.s
	@as hello.s -o hello.o
	@ld hello.o -o hello.out

gcc-hello: gcc-hello.s
	@gcc gcc-hello.s -o gcc-hello.out
	
clean:
	@rm hello.o hello.out gcc-hello.out
