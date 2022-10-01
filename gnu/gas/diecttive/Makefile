.PHONY: all clean
all:direct.o
	objdump -S direct.o > objdump.txt

#Makefile: ;
%.s:%.S
	gcc -S $< -o $@

%.o:%.s
	as $< -o $@

clean:
	rm -rf direct.s direct.o
