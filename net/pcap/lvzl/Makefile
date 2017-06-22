.PHONY:all

target = sniff_knock
all:$(target)

%:%.c
	@gcc -g $< -o $@ -lpcap

clean:
	@rm -rf $(target)

