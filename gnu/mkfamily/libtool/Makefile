
.PHONY: clean all default

.SUFFIXES: .lo .c

default: all

%.lo: %.c
	libtool --mode=compile gcc -g -O -c $<

libhello.la: hello.lo simplemath.lo
	#libtool --mode=link gcc -g -O -o $@ $^ -lm
	#libtool --mode=link gcc -module -shared -g -O -o $@ $^ -lm
	libtool --mode=link gcc -g -O -o $@ $^ -lm -rpath /root/noteGit/gnu/mkfamily/libtool/libs


all: libhello.la main.lo
	#libtool --mode=link gcc -g -O -o hello main.c libhello.la
	libtool --mode=link  gcc -shared -g -O -o hello main.c libhello.la

clean:
	-rm -rf .libs
	-rm *.lo
	-rm *.la
	-rm *.o
	-rm hello

