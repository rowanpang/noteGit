.PHONY: all clean
all:canceled

%:%.c
	@gcc -g -lpthread -std=gnu99 $< -o $@

clean:
	@rm canceled
