.PHONY:all
TARGETS=sender receiver
all:$(TARGETS)

LD_OPTS += $(shell pkg-config --libs openssl)
LD_OPTS += -lrt

%:%.c
	@gcc $(LD_OPTS) -g $< -o $@

clean:
	@-rm $(TARGETS)
