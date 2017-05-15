.PHONY:all clean

app = new simple
	
all:$(app)

%:%.c
	@gcc -g -pg $< -o $@
clean:
	@-rm -rf $(app)
