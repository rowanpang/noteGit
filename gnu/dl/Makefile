AP_ELF=main 
SO_ELF=msg.so

.PHONY:all

all:$(AP_ELF) $(SO_ELF)

%:%.c
	@gcc $< -o $@ -ldl

%.so:%.c
	@gcc -fPIC -shared -o $@ $<
	@cp $@ ./ldpathEnv/

.PHONY:clean
clean:
	@rm -rf $(AP_ELF) $(SO_ELF)
	@-rm ./ldpathEnv/*

