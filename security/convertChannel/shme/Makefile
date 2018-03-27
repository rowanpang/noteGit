.PHONY:all
TARGETS=sender receiver
all:$(TARGETS)

LD_OPTS += $(shell pkg-config --libs openssl)

%:%.c
	@gcc $(LD_OPTS) -g $< comm.c -o $@

clean:
	@-rm $(TARGETS)
