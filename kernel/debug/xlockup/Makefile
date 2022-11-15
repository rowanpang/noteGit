ifeq ($(KERNELRELEASE),)

KVER ?= `uname -r`
#KERNELDIR ?= /your/target/source/directory/
KERNELDIR ?= /lib/modules/$(KVER)/build
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

#rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

.PHONY: modules modules_install clean

else
    obj-m := softlock.o hardlock.o
endif
