.PHONY: all clean
all:once

%:%.c
	@gcc -g $< -o $@ -lpthread -std=gnu99

clean:
	@-rm simple
