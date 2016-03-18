.PHONY:all

all:a.out

a.out:main.c libhello.so
	gcc main.c -g -L./ -lhello -Wl,-rpath=`pwd`
	gcc main.c -g -L./ -lhello.nofPIC -Wl,-rpath=`pwd` -o a.out.nofPIC

libhello.so:hello.o
	gcc -shared ./hello.o -o libhello.so
	gcc -shared ./hello.o.nofPIC -o libhello.nofPIC.so

hello.o:hello.c
	gcc -g -c -fPIC hello.c -o hello.o
	gcc -g -c hello.c -o hello.o.nofPIC

.PHONY:clean
clean:
	-rm a.out a.out.nofPIC
	-rm libhello.so libhello.nofPIC.so
	-rm hello.o hello.o.nofPIC
