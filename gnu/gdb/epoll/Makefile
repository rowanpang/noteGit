.PHONY:all clean

TARGET = epoll
all:$(TARGET) simple

$(TARGET):main.c
	@gcc -gdwarf-2 -g3 -O -lpthread $< -o $@
	@eu-strip --remove-comment -f $@.info.sep $@

simple:simple.c
	@gcc -gdwarf-2 -g3 -O -lpthread $< -o $@
	@eu-strip --remove-comment -f $@.info.sep $@
clean:
	@-rm -rf $(TARGET)*
	@-rm -rf simple
	@-rm -rf simple.info.sep
