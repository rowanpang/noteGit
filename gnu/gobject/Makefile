.PHONY:all clean
all:main

OPT= $(shell pkg-config --libs --cflags gobject-2.0)

main:main.c sayHello.c sayHello.h
	gcc $(OPT) -Wall -o $@ $^

clean:
	@-rm -rf main
