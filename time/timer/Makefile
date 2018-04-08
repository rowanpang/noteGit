.PHONY:all
TARGETS=tick procFind notifyNone
all:$(TARGETS)

LD_OPTS += $(shell pkg-config --libs openssl)
LD_OPTS += -lrt

%:%.c
	@gcc $(LD_OPTS) -g -Wall $< -o $@

clean:
	@-rm $(TARGETS)
