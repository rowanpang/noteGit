

.PHONY:all
_all:all
# filter test
srctree := ./pangwz
src := top/dir/
src_1 := /top/dir/
kbuild-dir := $(if $(filter /%,$(src)),$(src),$(srctree)/$(src))
kbuild-dir_1 := $(if $(filter /%,$(src_1)),$(src_1),$(srctree)/$(src))
filter_test := $(filter pangwz,/pangwz pangwz1 apangwz pangwz)
filter_test_1 := $(filter %pangwz,/pangwz pangwz1 apangwz pangwz pangwz ppangwzz)
filter_test_2 := $(filter %pangwz%,/pangwz pangwz1 apangwz pangwz pangwz ppangwzz pangwz%)
filter_test_3 := $(filter 1,$(words pangwz ))
filter_test_3_num := $(words pangwz pangwz1 apangwz pangwz pangwz ppangwzz pangwz%)

.PHONY: prepare0,prepare1,prepare2

prepare0:
	@echo prepare0;

prepare1:
	@echo prepare1;

prepare2:
	@echo prepare2;

all:built-in.o prepare0 prepare1 prepare2
	@ echo target_v: $@ 
#	@ echo CURPATH:	$(CURDIR)
	@ echo dir CURPATH:	$(dir $(CURDIR))
#	@ echo kbuild-dir: $(kbuild-dir)
#	@ echo kbuild-dir_1: $(kbuild-dir_1)
#	@ echo filter_test: $(filter_test)
#	@ echo filter_test_1: $(filter_test_1)
#	@ echo filter_test_2: $(filter_test_2)
#	@ echo filter_test_3: $(filter_test_3)
#	@ echo filter_test_3_num: $(filter_test_3_num)
#	@ echo pangwz_num:$(1) $1
#
.PHONY:built-in.o,dir
built-in.o:dir;

dir:
	@echo "in target dir";

.PHONY:Makefile
Makefile: ;
