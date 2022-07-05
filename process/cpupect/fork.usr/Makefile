.PHONY: all clean

TARGETS=fork
all:$(TARGETS)

%:%.c
	@gcc -g $< -o $@ -std=gnu99

clean:
	@-rm $(TARGETS)

