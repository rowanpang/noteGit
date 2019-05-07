.PHONY: all clean
targets=simple simple.m64 rowan rowan.m64 case case.++ case.libc case.a align
all:$(targets)

align:align.c
	@gcc -g $< -o $@
case:case.c
	@gcc -g $< -o $@ -lpthread

case.a:case.c
	@gcc -static -g $< -o $@ -lpthread

case.++:case.c
	@g++ -g $< -o $@ -lpthread

case.libc:case.c
	@gcc -g $< -o $@

simple:test.cpp
	@g++ -g $< -o $@ -lpthread
simple.m64:test.cpp
	@g++ -m64 -g $< -o $@ -lpthread

rowan:test.rowan.cpp
	@g++ -g $< -o $@ -lpthread

rowan.m64:test.rowan.cpp
	@g++ -m64 -g $< -o $@ -lpthread
clean:
	@-rm $(targets)
