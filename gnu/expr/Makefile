.PHONY:all clean
all:main.c
	@gcc -g -O3 $< -o $(patsubst %.c,%,$<)
clean:
	@-rm -rf main
