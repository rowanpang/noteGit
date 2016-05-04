.PHONY:all,clean
all:hello

hello:hello.o
	@ld hello.o -o hello

hello.o:hello.s
	@as hello.s -o hello.o

clean:
	@rm hello.o hello
