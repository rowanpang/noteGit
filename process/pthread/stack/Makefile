.PHONY: all clean

TARGETS=stack
all:$(TARGETS)

%:%.c
	@gcc -g $< -o $@ -lpthread -std=gnu99

clean:
	@-rm $(TARGETS)
