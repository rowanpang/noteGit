.PHONY:all

all:main

%:%.c
	gcc -g3 $< -o $@
	gcc -g3 -S $< -o s.$@
	objdump -dS $@ > objdump.$@

clean:
	rm -rf main main.s main.objdump s.main objdump.main
