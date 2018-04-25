.PHONY:all
TARGETS=fib peach  pow factorial maxSubQueue
all:$(TARGETS)

LD_OPTS += $(shell pkg-config --libs openssl)

%:%.c
	@gcc $(LD_OPTS) -g -Wall $< -o $@

clean:
	@-rm $(TARGETS)
