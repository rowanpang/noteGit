.PHONY:all
all:
	@gcc -g struct.c -o x86_64.out
	@gcc -g -m32 struct.c -o i386.out
	@gcc -g -m32 -mregparm=3 struct.c -o i386.out.regArg

clean:
	@-rm i386.out x86_64.out
