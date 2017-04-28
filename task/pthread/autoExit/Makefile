.PHONY: all clean
all:simple

rowan_c_flags += -I./

%:%.c
	@gcc $(rowan_c_flags) -g $< -o $@ -lpthread -std=gnu99

clean:
	@-rm simple
