.PHONY: all clean

all:
	gcc -g main.c
	objdump --source --disassemble a.out > asm.txt

clean:
	@ -rm a.out
