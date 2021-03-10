.PHONY:all clean
all:valist

valist:main.c
	@gcc -gdwarf-2 -g3 -O main.c -o $@
	@eu-strip --remove-comment -f $@.info.sep $@
clean:
	@-rm -rf valist
	@-rm -rf valist.info.sep
	@-rm *.o
