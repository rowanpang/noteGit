.PHONY:all clean ln
all:a.out

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

clean:
	-rm -rf foo.o foo.o.fPIC hello.o* 
	-rm a.out* 
