.PHONY:all

all:a.out

a.out:main.c libhello.so
	gcc main.c -g -L./ -lhello -Wl,-rpath=`pwd`

libhello.so:hello.o
	gcc -shared ./hello.o -o libhello.so

hello.o:hello.c
	gcc -g -c -fPIC hello.c -o hello.o

.PHONY:clean
clean:
	-rm a.out
	-rm libhello.so
	-rm hello.o
