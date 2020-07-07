AP_ELF=main main.env
SO_ELF=msg.so

.PHONY:all

all:$(AP_ELF) $(SO_ELF)

%:%.c
	@gcc $< -o $@ -ldl

%.so:%.c
	@gcc -rdynamic -fPIC -shared -o $@ $<
	@cp $@ ./ldpathEnv/

.PHONY:clean
clean:
	@-rm -rf $(AP_ELF) $(SO_ELF)
	@-rm ./ldpathEnv/*
	@-rm a.out

