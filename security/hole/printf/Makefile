.PHONY:all
TARGETS=main stack
all:$(TARGETS)

#LD_OPTS += $(shell pkg-config --libs openssl)
CC_OPTS += -Wall -g
CC_OPTS += -fno-stack-protector -z execstack
CC_OPTS += -m32

%:%.c
	@gcc $(LD_OPTS) $(CC_OPTS) $< -o $@

clean:
	@-rm $(TARGETS)
