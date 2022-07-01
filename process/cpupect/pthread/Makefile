.PHONY: all clean
all:simple

%:%.c
	@gcc -g $< -o $@ -lpthread -std=gnu99

clean:
	@-rm simple
