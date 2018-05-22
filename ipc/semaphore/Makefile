CC       = gcc
LINK     = gcc
CFLAGS   = -g -c
INCPATH  =
LIBS     =
OBJECTS_DIR = ./
TARGET=A B

####### Implicit rules
.SUFFIXES: .c

all: $(TARGET)

%:%.c
	$(LINK) -o $@ $<

install:
	mv $(TARGET) ../bin
clean:
	rm -rf *.o
	-rm -rf $(TARGET)
