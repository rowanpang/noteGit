.PHONY: all clean
all:simple simple.ng

simple:
	@g++ -m64 -g test.cpp -o $@ -lpthread
simple.ng:
	@g++ -g test.cpp -o $@ -lpthread

clean:
	@-rm simple simple.ng
