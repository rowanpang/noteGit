.PHONY:all
TARGETS=niceTest
all:$(TARGETS)

LD_OPTS += $(shell pkg-config --libs openssl) -lm

%:%.c
	@gcc $(LD_OPTS) -g $< -o $@

clean:
	@-rm $(TARGETS)
