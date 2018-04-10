.PHONY:all clean hello
all:hello

hello:hello.s
	@as hello.s -o hello.o
	@ld hello.o -o hello

clean:
	@-rm hello.o hello
