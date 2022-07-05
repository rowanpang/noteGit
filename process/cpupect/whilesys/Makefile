.PHONY: all clean

TARGETS=sys
all:$(TARGETS)

%:%.c
	@gcc -g $< -o $@ -std=gnu99

clean:
	@-rm $(TARGETS)

