.PHONY:all
TARGETS = hello
all:$(TARGETS)

%:%.go
	@go build -o $@ $<

.PHONY:clean

clean:
	@-rm -rf $(TARGETS)
