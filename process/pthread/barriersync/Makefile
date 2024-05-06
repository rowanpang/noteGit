.PHONY: all clean
all:barrier

%:%.c
	@gcc -g $< -o $@ -lpthread -std=gnu99

clean:
	@-rm barrier
