.PHONY:all

all:static

%:%.c
	gcc -static $< -o $@
.PHONY:clean
clean:
	-rm static
