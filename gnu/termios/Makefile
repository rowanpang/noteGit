.PHONY:all
all:attr tty pollAttr

tty:tty.c
	@gcc tty.c -o tty

attr:attr.c
	@gcc attr.c -o attr

pollAttr:pollAttr.c
	@gcc pollAttr.c -o pollAttr


.PHONY:clean
clean:
	@rm attr
	@rm tty
	@rm pollAttr
