.PHONY:all

all:a.out

a.out:main.c libhello.so
	gcc main.c -L./ -lhello -Wl,-rpath=`pwd`

libhello.so:hello.s
	gcc -shared ./hello.s -o libhello.so

hello.s:hello.c
	gcc -S -fPIC hello.c -o hello.s

.PHONY:clean
clean:
	rm a.out 
	rm libhello.so
	rm hello.s
