.PHONY:all

#compile
CC := gcc
CC_OPTS :=

define make-depend-compile
	@echo "[CC] $1"
	@mkdir -p $(dir $2)
	@$(CC) -MM -MF $3 -MP -MT $2 $(CC_OPTS) $1
	@$(CC) $(CC_OPTS) -c -o $2 $1
endef

OBJPATH := .
SRC_C := $(wildcard *.c)
OBJ_C := ${SRC_C:%.c=$(OBJPATH)/%.o}
OBJ_C_D := ${OBJ_C:.o=.d}

OBJ := $(OBJ_C)
OBJ_D := $(OBJ_C_D)

#elf,ld
AP_ELF := a.out
AP_MAP := a.map

ifneq ($(AP_MAP),)
    LD_OPTS += -Wl,-Map=$(AP_MAP)
endif

#default goal
all:$(AP_ELF)

$(AP_ELF):$(OBJ)
	@echo "[LD] $@"
	@$(CC) $(LD_OPTS) -Wl,--start-group $^ -Wl,--end-group  -o $@ 

$(OBJPATH)/%.o: %.c
	$(call make-depend-compile,$<,$@,$(subst .o,.d,$@))	

.PHONY:clean
clean:
	@rm -rf $(AP_ELF) $(AP_MAP) $(OBJ) $(OBJ_D)

-include $(OBJ_D)
