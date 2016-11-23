.PHONY:all
all:main.c
	@gcc -g main.c -o inst
	@readelf --all --wide inst > log-elf
	@objdump --source --disassemble inst > log-objdump

.PHONY:clean
clean:
	@rm inst log-*
