.PHONY:all

all:a.out a.at.out

a.out:main.c libhello.so
	gcc main.c -g -L./ -lhello -Wl,-rpath=`pwd` -o $@

a.at.out:main.c libhello.so
	/opt/at12.0/bin/gcc main.c -g -L./ -lhello -Wl,-rpath=`pwd` -o $@

libhello.so:hello.o
	gcc -shared ./hello.o -o libhello.so

hello.o:hello.c
	gcc -g -c -fPIC hello.c -o hello.o

.PHONY:clean
clean:
	-rm a.out a.at.out
	-rm libhello.so
	-rm hello.o
