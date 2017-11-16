.PHONY:all
all:normal memleak strdupleak strdupexec

%:%.c
	@gcc -g -Wall $< -o $@

clean:
	@-rm normal memleak strdupleak
