.PHONY:all clean

TARGET = epoll
all:$(TARGET) simple

$(TARGET):epoll.c
	@gcc -gdwarf-2 -g3 -O -lpthread $< -o $@
	@eu-strip --remove-comment -f $@.info.sep $@

simple:simple.c
	@gcc -gdwarf-2 -g3 -O -lpthread $< -o $@
	@eu-strip --remove-comment -f $@.info.sep $@
clean:
	@-rm -rf epoll
	@-rm -rf epoll.info.sep
	@-rm -rf simple
	@-rm -rf simple.info.sep
