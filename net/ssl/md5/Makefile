.PHONY:all
TARGETS=md5
all:$(TARGETS)

LD_OPTS += $(shell pkg-config --libs openssl)

%:%.c
	@gcc $(LD_OPTS) -g $< -o $@

clean:
	@-rm $(TARGETS)
