.PHONY:all

targets=dynamiclib staticlib staticelf

all:$(targets)

staticelf:main.c libhello.a
	gcc main.c -static -g -L./ -lhello -Wl,-rpath=`pwd` -o $@

staticlib:main.c libhello.a
	gcc main.c -g -L./ -Wl,-Bstatic -lhello -Wl,-Bdynamic -o $@
	gcc main.c -g -L./ -l:libhello.a -o $@.2

dynamiclib:main.c libhello.so
	gcc main.c -g -L./ -lhello -Wl,-rpath=`pwd` -o $@

libhello.so:hello.o
	gcc -shared $< -o $@

libhello.a:hello.o
	ar -rcs $@ $<

hello.o:hello.c
	gcc -g -c -fPIC $< -o $@

.PHONY:clean
clean:
	-rm $(targets) staticlib.2
	-rm libhello.so
	-rm libhello.a
	-rm hello.o
