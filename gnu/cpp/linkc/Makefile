.PHONY: all

all: nest dyn

main.o: main.cpp
	g++ -fPIC -c $^
sub.o: sub.c
	g++ -fPIC -c $^
add.o: add.c
	gcc -fPIC -c $^

libadd.so: add.o
	gcc -shared add.o -o libadd.so

nest: main.o sub.o add.o
	g++ main.o sub.o add.o -o nest
	./nest
	echo ""

dyn: main.o sub.o libadd.so
	g++ main.o sub.o -L./ -ladd -Wl,-rpath=`pwd` -o dyn
	./dyn

clean:
	- rm -rf *.o *.so
	- rm -f nest dyn
