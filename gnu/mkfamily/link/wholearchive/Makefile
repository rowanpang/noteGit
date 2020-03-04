.PHONY:all

all:dynelf dynelf.all dynelf.msg.direct

dynelf:main.c libdyn.so
	-gcc main.c -g -L./ -ldyn -Wl,-rpath=`pwd` -o $@

dynelf.all:main.c libdyn.all.so
	gcc main.c -g -L./ -ldyn.all -Wl,-rpath=`pwd` -o $@

dynelf.msg.direct:main.c libdyn.so
	gcc -DNO_SAYHELLO main.c -g -L./ -ldyn -Wl,-rpath=`pwd` -o $@

libdyn.so:dyn.o libhellomsg.a
	gcc -shared $^ -o $@

libdyn.all.so:dyn.o libhellomsg.a
	gcc -shared dyn.o -Wl,--whole-archive libhellomsg.a -Wl,--no-whole-archive -o libdyn.all.so

libhellomsg.a:hello.o msg.o
	ar -rcs $@ $^

%.o:%.c
	gcc -g -c -fPIC $< -o $@

.PHONY:clean
clean:
	-rm dynelf*
	-rm libdyn*
	-rm libhellomsg.a
	-rm *.o
