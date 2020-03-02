.PHONY:all clean ln
all:soname.1.out soname.2.out nosoname.out 

soname.1.out:hello.o ln
	@ln -f -s libfoo.so.1.1 libfoo.so
	@gcc hello.o -lfoo -o soname.1.out -Wl,-L. -Wl,-rpath=`pwd`/rpath 

soname.2.out:hello.o ln
	@ln -f -s libfoo.so.1.2 libfoo.so
	@gcc hello.o -lfoo -o soname.2.out -Wl,-L. -Wl,-rpath=`pwd`/rpath 

nosoname.out:hello.o ln
	@ln -f -s libfoo.nosoname.so libfoo.so
	@gcc hello.o -lfoo -o nosoname.out -Wl,-L. -Wl,-rpath=`pwd`/rpath

hello.o:hello.c
	@gcc -c $^ -o $@

foo.o:foo.c
	@gcc -c foo.c -o foo.o -fPIC

libfoo.so.1.1:foo.o
	@ld -shared foo.o -o libfoo.so.1.1 -soname=libfoo.so.1

libfoo.nosoname.so:foo.o
	@ld -shared foo.o -o libfoo.nosoname.so 

libfoo.so.1.2:foo2.c
	@gcc -shared foo2.c -o libfoo.so.1.2 -fPIC -Wl,-soname=libfoo.so.2

ln:libfoo.so.1.1 libfoo.so.1.2 libfoo.nosoname.so
	@mkdir -p `pwd`/rpath
	@ln -f -s `pwd`/libfoo.so.1.1 `pwd`/rpath/libfoo.so.1
	@ln -f -s `pwd`/libfoo.so.1.2 `pwd`/rpath/libfoo.so.2
	#@ln -f -s `pwd`/libfoo.nosoname.so `pwd`/rpath/libfoo.nosoname.so

clean:
	@-rm -rf soname.1.out soname.2.out nosoname.out
	@-rm -rf foo.o hello.o libfoo.so.1.1 libfoo.so.1.2 libfoo.so
	@-rm -rf libfoo.nosoname.so 
	@-rm -rf libfoo.so*
	@-rm -rf rpath
