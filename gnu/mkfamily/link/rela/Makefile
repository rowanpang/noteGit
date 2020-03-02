.PHONY:all clean ln
all:a.out a.out.so

a.out:hello.o foo.o 
	gcc hello.o foo.o -o a.out
	gcc hello.o foo.o.fPIC -o a.out.foo.fPIC
	gcc hello.o.fPIC foo.o -o a.out.hello.fPIC
	gcc hello.o.fPIC foo.o.fPIC -o a.out.fPIC

hello.o:hello.c
	gcc -g -c $^ -o $@
	gcc -g -c hello.c -o hello.o.fPIC -fPIC

foo.o:foo.c
	gcc -g -c foo.c -o foo.o.fPIC -fPIC
	gcc -g -c foo.c -o foo.o

a.out.so:hello.c libfoo.so hello.o
	gcc -g hello.c -L. -lfoo -Wl,-rpath=`pwd` -o a.out.so
	gcc -g hello.o -L. -lfoo -Wl,-rpath=`pwd` -o a.out.so.o
	gcc -g hello.o.fPIC -L. -lfoo -Wl,-rpath=`pwd` -o a.out.so.o.fPIC

libfoo.so:foo.c
	gcc -g -shared -fPIC foo.c -o libfoo.so

clean:
	-rm -rf foo.o foo.o.fPIC hello.o* 
	-rm a.out* 
	-rm libfoo.so
