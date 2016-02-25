ifeq ($(KERNELRELEASE),)

KVER ?= `uname -r`
#KERNELDIR ?= /your/target/source/directory/
KERNELDIR ?= /lib/modules/$(KVER)/build
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

.PHONY: modules modules_install clean

else
    obj-m := memdev.o
endif
