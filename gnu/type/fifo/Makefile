.PHONY: all clean

all:fifo

%:%.c
	gcc -g3 -O0 -o $@ $< -lpthread

clean:
	-rm -f fifo
