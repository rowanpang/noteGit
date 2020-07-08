AP_ELF=main
SO_ELF=libmsg.so

.PHONY:all

all:$(AP_ELF) $(SO_ELF)

%:%.c
	@gcc -g $< -o $@

libmsg.so:msg.c
	@gcc -g -fPIC -shared -o $@ $<

.PHONY:clean
clean:
	@rm -rf $(AP_ELF) $(SO_ELF)
