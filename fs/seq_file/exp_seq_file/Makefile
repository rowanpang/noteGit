# Rules for making the NTFS driver.

MAKE := make
KVER := $(shell uname -r)
KSRC := /lib/modules/$(KVER)/build

ifneq ($(KERNELRELEASE),)
obj-m := seq_file.o

else
.PHONY:all,modules
all:modules

modules:
	$(MAKE) -C $(KSRC) M=$(shell pwd)  modules

.PHONY:clean
#rm -rf *.mod.c *.mod *.o .*.cmd *.ko 
clean:
	$(MAKE) -C $(KSRC) M=$(shell pwd) clean 
endif
