.PHONY:all
TARGETS=rttest
all:$(TARGETS)

LD_OPTS += $(shell pkg-config --libs openssl) -lm

%:%.c
	@gcc $(LD_OPTS) -g $< -o $@

clean:
	@-rm $(TARGETS)
