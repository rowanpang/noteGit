.PHONY:all
all:normal memleak strdupleak strdupexec unInit

%:%.c
	@gcc -g -Wall $< -o $@

clean:
	@-rm normal memleak strdupleak
