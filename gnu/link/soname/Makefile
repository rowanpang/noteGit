.PHONY:all clean ln
all:a.out libfoo2.so

a.out:hello.o libfoo.so ln
	gcc hello.o -lfoo -o soname -Wl,-L. -Wl,-rpath=`pwd`/rpath 
	gcc hello.o -lfoo_nosoname -o nosoname -Wl,-L. -Wl,-rpath=`pwd`

hello.o:hello.c
	gcc -c $^ -o $@

libfoo.so:foo.o
	ld -shared foo.o -o libfoo.so -soname=libfoo.so.1
	ld -shared foo.o -o libfoo_nosoname.so 

foo.o:foo.c
	gcc -c foo.c -o foo.o -fPIC

ln:libfoo.so
	mkdir -p `pwd`/rpath
	ln -s `pwd`/libfoo.so `pwd`/rpath/libfoo.so.1.bak
	ln -s `pwd`/libfoo2.so `pwd`/rpath/libfoo.so.1

libfoo2.so:foo2.c
	gcc -shared foo2.c -o libfoo2.so -fPIC

clean:
	rm soname nosoname
	rm -rf foo.o hello.o libfoo.so
	rm libfoo2.so 
	rm libfoo_nosoname.so 
	rm -rf rpath
