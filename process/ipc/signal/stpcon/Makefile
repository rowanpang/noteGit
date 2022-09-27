.PHONY:all clean
elfObj = sigtstp ixon
all:$(elfObj)

%:%.c
	@gcc -g $< -o $@
clean:
	@-rm -rf $(elfObj)
