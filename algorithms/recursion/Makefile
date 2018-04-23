.PHONY:all
TARGETS=fib fib1 fib2 peach  pow factorial
all:$(TARGETS)

LD_OPTS += $(shell pkg-config --libs openssl)

%:%.c
	@gcc $(LD_OPTS) -g -Wall $< -o $@

clean:
	@-rm $(TARGETS)
