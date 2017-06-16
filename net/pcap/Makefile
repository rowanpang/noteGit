.PHONY:all

target = base
all:$(target)

%:%.c
	@gcc -g $< -o $@ -lpcap

clean:
	@rm -rf $(target)

