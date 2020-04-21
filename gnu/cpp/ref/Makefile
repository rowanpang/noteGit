.PHONY:all
TARGETS=ref
all:$(TARGETS)

#LD_OPTS += $(shell pkg-config --libs openssl)

ref:ref.cpp
	@g++ $(LD_OPTS) -g $< -o $@
#@g++ $(LD_OPTS) -g -std=c++98 $< -o $@.c98

clean:
	@-rm $(TARGETS)
