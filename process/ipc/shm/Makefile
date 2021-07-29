CC       = gcc
LINK     = gcc
CFLAGS   = -g -c
INCPATH  =
LIBS     =
OBJECTS_DIR = ./
TARGET=shm shmread shmwrite

####### Implicit rules

.SUFFIXES: .c
.c.o:
	$(CC) $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: $(TARGET)

####### Shared Memory  Examples

shm:shm.o
	$(LINK) -o $@ $<

shmread:shmread.o
	$(LINK) -o $@ $<

shmwrite:shmwrite.o
	$(LINK) -o $@ $<
###### Clean Object files

install:
	mv $(TARGET) ../bin
clean:
	rm -rf *.o
	-rm -f $(TARGET)
