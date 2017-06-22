AP_ELF=main verifypasswd

SO_ELF=msg.so

.PHONY:all

all:$(AP_ELF) $(SO_ELF)

%:%.c
	@gcc $< -o $@

%.so:%.c
	@gcc -fPIC -shared -o $@ $<

.PHONY:clean
clean:
	@rm -rf $(AP_ELF) $(SO_ELF)

