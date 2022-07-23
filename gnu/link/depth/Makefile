.PHONY:all

all:main

main: main.c libhello.so libshow.so libshow2.so
	gcc -Wall -ggdb main.c -L./ -lhello -lshow -l show2 -Wl,-rpath=`pwd` -o $@

libhello.so:hello.c libshow.so
	gcc -fPIC -shared hello.c -L./ -lshow -o $@

libshow.so:show.c libshow2.so
	gcc -fPIC -shared show.c -L./ -lshow2 -o $@

libshow2.so:show2.c
	gcc -fPIC -shared show2.c -o $@

.PHONY:clean
clean:
	@-rm main
	@-rm *.so*
	@-rm *.s
	@-rm *.o
