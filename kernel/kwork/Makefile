TEST_MOD_NAME = modexp_kwork

MAKE := make
KVER := $(shell uname -r)
KSRC := /lib/modules/$(KVER)/build

ifneq ($(KERNELRELEASE),)
#auto find the .c file under the objdir,and got the filename without dir
src_c := $(notdir $(wildcard $(src)/*.c))
obj_o := $(patsubst %.c, %.o, $(src_c))

obj-m += $(TEST_MOD_NAME).o
$(TEST_MOD_NAME)-objs += $(obj_o)
#cflags-y += -E

else
#auto find the .c file under the objdir,and got the filename without dir
src=$(shell pwd)
src_c := $(notdir $(wildcard $(src)/*.c))
obj_o := $(patsubst %.c, %.o, $(src_c))
obj_i := $(patsubst %.c, %.i, $(src_c))

.PHONY:all modules
ifeq ("$(origin FORCE)", "command line")
    FORCE_INSTALL = $(FORCE)		#The ^blank should be space not table
endif
ifndef FORCE_INSTALL
    FORCE_INSTALL = 1
endif

all:modules
modules:
	$(MAKE) -C $(KSRC) M=$(shell pwd)  modules

.PHONY:clean
clean:
	$(MAKE) -C $(KSRC) M=$(shell pwd)  clean
	@rm -rf $(obj_i)
#rm -rf *.mod.c *.mod *.o .*.cmd *.ko

.PHONY:preProcess
preProcess:
	$(MAKE) -C $(KSRC) M=$(shell pwd) $(obj_i)

.PHONY:test
test:
	@echo $(PWD)
	@echo $(CURDIR)
	@echo $(obj_i)

$(TEST_MOD_NAME).ko:modules

.PHONY:install,uninstall
install:$(TEST_MOD_NAME).ko
	@if [ `lsmod | grep "$(TEST_MOD_NAME)" -c` -gt 0 ];then \
		echo "already installed,force:$(FORCE_INSTALL)";	\
		if [ "$(FORCE_INSTALL)" -eq 1 ];then \
			echo "force install.fir:uninstall,sec:install"; \
			make uninstall; \
			sudo insmod $(TEST_MOD_NAME).ko; \
		fi \
	else \
		echo "not install";	\
		sudo insmod $(TEST_MOD_NAME).ko; \
	fi

uninstall:
	@sudo rmmod $(TEST_MOD_NAME).ko

.PHONY: help
help:
	@echo
	@echo "**********************************************************"
	@echo "make:compile the modules"
	@echo "make clean: do clean"
	@echo
	@echo "make install FORCE=1:force reinstall mod even already install"
	@echo "make install FORCE=0:not do reinstall"
	@echo "make install:same as FORCE=1"
	@echo "make uninstall: do rmmod"
	@echo "**********************************************************"
	@echo
endif
