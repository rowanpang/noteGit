.PHONY:all
TARGETS=pf hmac digest
all:$(TARGETS)

LD_OPTS += $(shell pkg-config --libs openssl)

%:%.c
	@gcc $(LD_OPTS) -g $< -o $@

clean:
	@-rm $(TARGETS)
