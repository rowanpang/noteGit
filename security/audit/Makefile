.PHONY:all
#make -n to see diff
#lflags = $(shell pkg-config --libs audit)
lflags = `pkg-config --libs audit`
all:main.c
	@gcc -g main.c $(lflags) -o auditTest

.PHONY:clean
clean:
	@-rm auditTest
