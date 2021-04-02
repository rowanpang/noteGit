.PHONY:all
all:
	@gcc -g castimply.c -o x86_64.out
	@gcc -g -m32 castimply.c -o i386.out
	@gcc -g -m32 -mregparm=3 castimply.c -o i386.out.regArg

clean:
	@-rm i386.out* x86_64.out
