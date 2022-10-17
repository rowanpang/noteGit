.PHONY: all

all:simple

simple:simple.o
	gcc simple.o -o simple

%.o:%.c
	gcc -g -c $< -o $@

.PHONY: clean

clean:
	-rm -rf simple simple.o
