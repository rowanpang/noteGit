.PHONY:all clean hello
all:hello gcc-hello nolibc

nolibc:nolibc.s
	@as nolibc.s -o nolibc.o
	@ld nolibc.o -o nolibc.out

hello:hello.s
	@as hello.s -o hello.o
	@ld hello.o -o hello.out

gcc-hello: hello.s
	@gcc hello.s -o gcc-hello.out

clean:
	@rm hello.o hello.out gcc-hello.out nolibc.o nolibc.out
