.PHONY:all
TARGETS=is_integral
all:$(TARGETS)

#LD_OPTS += $(shell pkg-config --libs openssl)

%:%.c
	@g++ $(LD_OPTS) -g $< -o $@

clean:
	@-rm $(TARGETS)
