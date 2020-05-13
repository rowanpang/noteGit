.PHONY:all

all:
	gcc -g -rdynamic main.c dump.c add.c
	gcc -g -rdynamic add.c -fPIC -shared -o libadd.so
	gcc -g -rdynamic main.c dump.c -L. -ladd -Wl,-rpath=. -o backtrace

clean:
	@-rm a.out
