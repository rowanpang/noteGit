.PHONY:all
all:a.out a.out.s

a.out:main.c foo1.c foo2.c
	gcc $^ -o $@


a.out.s: main.o foo.o 
	gcc $^ -o $@

main.o:main.c
	gcc -o main.o -c main.c

#-r,output relocate files,as input for ld
foo.o:foo1.o foo2.o
	ld $^ -r -o $@

foo1.o:foo1.c
	gcc -o foo1.o -c foo1.c	

foo2.o:foo2.c
	gcc -o foo2.o -c foo2.c	

#disable implicit rules for %.o
%.o: ;

.PHONY:clean
clean:
	-@rm a.out*
	-@rm *.o
