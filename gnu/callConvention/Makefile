.PHONY:all
all:
	@gcc -g struct.c -o x86_64.out
	@objdump --source --disassemble x86_64.out > x86_64.objdump
	@gcc -g -m32 struct.c -o i386.out
	@objdump --source --disassemble i386.out > i386.objdump
	@gcc -g -m32 -mregparm=3 struct.c -o i386.out.regArg
	@objdump --source --disassemble i386.out.regArg > i386.regArg.objdump

clean:
	@-rm i386.out* x86_64.out
	@-rm *.objdump
