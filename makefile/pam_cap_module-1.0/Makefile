conf_file := conf
login_file := login

DEBUG = true

ROOT = ./out
OBJDIR = $(ROOT)/OBJ
OBJPATH = $(OBJDIR)
BINDIR = $(ROOT)/BIN
BINPATH = $(BINDIR)

ifeq ($(DEBUG),true)
	INSDIR_so = $(ROOT)/lib64/security/
else
	INSDIR_so = ./lib64/security/
endif

###################################
# src file include
SRC_FILE = ./pam_capset_auth.c
SRC_C = $(filter %.c, $(SRC_FILE))

OBJ_C = ${SRC_C:%.c=$(OBJPATH)/%.o}
OBJ = $(OBJ_C)

INC_DIR = 

MODULE = $(BINPATH)/libpam_cap_set.so
MODULE_NOTDIR = $(notdir $(MODULE))

######################################
# dep file gen auto,here just include
OBJ_DEP_C = $(OBJ_C:%.o=%.d)
OBJ_DEP = $(OBJ_DEP_C)
-include $(OBJ_DEP)

####################################
# compile tools
CC = gcc

#####################################
# top target
.PHONY: $(PHONY) 
PHONY:=first

first:$(MODULE)
	@echo "--------finish-------"

PHONY+=predep
predep:
	@echo "$(SRC_C)"
	@echo "$(OBJ_C)"
	@echo "$(OBJPATH)"
	@echo "$(OBJ_DEP)"	
	@echo "finish predep"

$(MODULE):sharelib
	@echo "finish--$@"

PHONY+=sharelib
sharelib:$(OBJ_C)
	@mkdir -p $(dir $(MODULE))
	@$(CC) -shared -o $(MODULE) -lpam
	@echo "finish--$@"

$(OBJPATH)/%.o: %.c
	$(call make-depend-compile,$<,$@,$(subst .o,.d,$@))


install: $(MODULE)
	@mkdir -p $(INSDIR_so)
	install $(MODULE) $(INSDIR_so)/$(MODULE_NOTDIR)

uninstall: 
	rm -f $(INSDIR)/$(MODULE_NOTDIR)

clean:
	rm -rf $(OBJPATH) $(BINPATH)

#$(call make-depend-compile,source-file,object-file,depend-file)
define make-depend-compile
	@echo "[cc] $1"
	@mkdir -p $(dir $2)
	@$(CC) -MM -MF $3 -MP -MT $2 $(CC_OPTS) $1
	@$(CC) $(CC_OPTS) -o $2 -c $1
endef		
