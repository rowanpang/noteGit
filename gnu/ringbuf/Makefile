.PHONY: all clean
TARGETS = simple cond
all:$(TARGETS)

%:%.c
	@gcc -g $< -o $@ -lpthread -std=gnu99

clean:
	@-rm $(TARGETS)
