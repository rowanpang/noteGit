.PHONY:all clean ln
all:binelf hello.dyn binelf.oo

binelf:hello.c foo.c
	gcc -g hello.c foo.c -o $@

binelf.oo:hello.o foo.o
	gcc hello.o foo.o -o binelf.oo
	gcc hello.fpic.o foo.fpic.o -o binelf.fpic
	gcc hello.o foo.fpic.o -o binelf.foo.fpic
	gcc hello.fpic.o foo.o -o binelf.hello.fpic

hello.o:hello.c
	gcc -g -c $^ -o $@
	gcc -g -fPIC -c hello.c -o hello.fpic.o

foo.o:foo.c
	gcc -g -c foo.c -o foo.o
	gcc -g -fPIC -c foo.c -o foo.fpic.o

hello.dyn:hello.c libfoo.so hello.o
	gcc -g hello.c -L. -lfoo -Wl,-rpath=`pwd` -o binelf.dyn.hello.c
	gcc -g hello.o -L. -lfoo -Wl,-rpath=`pwd` -o binelf.dyn.hello.o
	gcc -g hello.fpic.o -L. -lfoo -Wl,-rpath=`pwd` -o binelf.dyn.hello.fpic.o

libfoo.so:foo.c
	gcc -g -shared -fPIC foo.c -o libfoo.so
#gcc -g -shared foo.o -o libfoo.o.so 		#使用非fPIC编译好的.o生成so时报错,提示需要fPIC.
#/usr/bin/ld: foo.o: relocation R_X86_64_32 against `.rodata' can not be used when making a shared object; recompile with -fPIC
#collect2: error: ld returned 1 exit status
#make: *** [Makefile:26: libfoo.so] Error 1

objdump:all
	objdump -S binelf > objdump.binelf.txt
	objdump -S binelf.oo > objdump.binelf.oo.txt
	objdump -S binelf.hello.fpic > objdump.binelf.hello.fpic.txt
	objdump -S hello.o > objdump.hello.o.txt
	objdump -S hello.fpic.o > objdump.hello.fpic.o.txt
	objdump -S binelf.dyn.hello.c > objdump.binelf.dyn.hello.c.txt
	objdump -S binelf.dyn.hello.o > objdump.binelf.dyn.hello.o.txt
	objdump -S binelf.dyn.hello.fpic.o > objdump.binelf.dyn.hello.fpic.o.txt
	objdump -S libfoo.so > objdump.libfoo.so.txt

clean:
	-rm -rf foo.o foo.fpic.o hello.o hello.*.o
	-rm binelf*
	-rm libfoo.so
