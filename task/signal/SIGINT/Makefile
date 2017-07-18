.PHONY:all clean
elfObj = justSleep  pthread fork
all:$(elfObj)

%:%.c
	@gcc -g $< -o $@ -lpthread
clean:
	@-rm -rf $(elfObj)
