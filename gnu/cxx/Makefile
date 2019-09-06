.PHONY:all
TARGETS=vector
all:$(TARGETS)

#LD_OPTS += $(shell pkg-config --libs openssl)

%:%.c
	@g++ $(LD_OPTS) -g $< -o $@

clean:
	@-rm $(TARGETS)
