.PHONY: all clean
all:simple simple.m64 rowan rowan.m64 case

case:
	@g++ -g case.c -o $@ -lpthread
simple:
	@g++ -g test.cpp -o $@ -lpthread
simple.m64:
	@g++ -m64 -g test.cpp -o $@ -lpthread

rowan:
	@g++ -g test.rowan.cpp -o $@ -lpthread
rowan.m64:
	@g++ -m64 -g test.rowan.cpp -o $@ -lpthread
clean:
	@-rm simple* rowan* case
