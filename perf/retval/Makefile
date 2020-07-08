AP_ELF=dynso
SO_ELF=libmsg.so

.PHONY:all

all:$(AP_ELF) $(SO_ELF)

%:%.c
	@gcc -g $< -o $@ -lmsg -L. -Wl,-rpath=`pwd`

libmsg.so:libmsg.c
	@gcc -g -fPIC -shared -o $@ $<

.PHONY:clean
clean:
	@rm -rf $(AP_ELF) $(SO_ELF)
