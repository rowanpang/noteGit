CC       = gcc
LINK     = gcc
CFLAGS   = -g -c
INCPATH  =
LIBS     =
OBJECTS_DIR = ./
TARGET=blockTest sigAction sig3


all: $(TARGET)

%:%.c
	@gcc -g $< -o $@


install:
	mv $(TARGET) ../bin
clean:
	rm -rf *.o
	-rm -rf $(TARGET)
