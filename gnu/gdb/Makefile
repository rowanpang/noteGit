.PHONY:all clean
all:dbg

dbg:main.c
	@gcc -gdwarf-2 -g3 -O main.c -o $@
	@eu-strip --remove-comment -f $@.info.sep $@
clean:
	@-rm -rf a.out
	@-rm -rf dbg
	@-rm -rf dbg.info.sep
	@-rm *.o
