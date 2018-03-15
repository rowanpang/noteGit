.PHONY:all clean

app = new simple ptyFork

all:$(app)

%:%.c
	@gcc -g -pg -lutil $< -o $@
clean:
	@-rm -rf $(app)
