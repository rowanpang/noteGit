.PHONY:all
TARGETS=is_integral auto using lambda
all:$(TARGETS)

#LD_OPTS += $(shell pkg-config --libs openssl)

%:%.c
	@g++ $(LD_OPTS) -g $< -o $@

clean:
	@-rm $(TARGETS)
