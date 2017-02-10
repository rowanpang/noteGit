.PHONY:all clean
all:hello

hello:main.c
	@gcc -g main.c -o $@
	@cp $@ $@.org
	@eu-strip --remove-comment -f $@.debug $@
	@cp $@ $@.mini
	@./minidebug.sh $@.debug $@.mini
clean:
	@-rm -rf a.out
	@-rm -rf hello hello.*
	@-rm -rf tmp.*
