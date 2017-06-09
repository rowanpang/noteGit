.PHONY:all clean
elfObj = sigtstp
all:$(elfObj)

%:%.c
	@gcc -g $< -o $@
clean:
	@-rm -rf $(elfObj)
