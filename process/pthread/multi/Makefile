.PHONY: all clean
all:multi

%:%.c
	@gcc -g3 $< -o $@ -lpthread -std=gnu99

clean:
	@-rm multi
