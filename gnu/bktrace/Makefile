.PHONY:all

all:
	gcc -g -rdynamic main.c dump.c add.c
	gcc -g -rdynamic add.c -fPIC -shared -o libadd.so
	gcc -g -rdynamic main.c dump.c -L. -ladd -Wl,-rpath=. -o backtrace


clean:
	@-rm a.out
	@-rm libadd.so
	@-rm backtrace


#man gcc
#-rdynamic
   #Pass the flag -export-dynamic to the ELF linker, on targets that support it. This instructs the linker to add all symbols, not only used ones, to the dynamic symbol table. This option
   #is needed for some uses of "dlopen" or to allow obtaining backtraces from within a program.

