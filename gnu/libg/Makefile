.PHONY:all clean
all:gtest

gtest:main.c
	@gcc -g -O main.c $(shell pkg-config --cflags glib-2.0) $(shell pkgconf --libs glib-2.0) -o $@
	@eu-strip --remove-comment -f $@.debug $@
clean:
	@-rm -rf a.out
	@-rm -rf gtest
	@-rm -rf gtest.debug
	@-rm *.o
