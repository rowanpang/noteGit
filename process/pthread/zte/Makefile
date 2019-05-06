.PHONY: all clean
all:simple simple.m64 rowan rowan.m64 case case.++ case.libc

case:
	@gcc -g case.c -o $@ -lpthread

case.++:
	@g++ -g case.c -o $@ -lpthread

case.libc:
	@gcc -g case.c -o $@

simple:
	@g++ -g test.cpp -o $@ -lpthread
simple.m64:
	@g++ -m64 -g test.cpp -o $@ -lpthread

rowan:
	@g++ -g test.rowan.cpp -o $@ -lpthread
rowan.m64:
	@g++ -m64 -g test.rowan.cpp -o $@ -lpthread
clean:
	@-rm simple* rowan* case.++ case case.libc
