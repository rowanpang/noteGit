CC       = gcc
LINK     = gcc -lm
CFLAGS   = -g -c
INCPATH  =
LIBS     =
OBJECTS_DIR = ./

TARGET = pipeSimple pipelimit popenPlot fifoOpen \
	 fifoRead fifoWrite

####### Implicit rules

####### Build rules

all: $(TARGET)

%:%.c
	@gcc -g -lm $< -o $@

###### Clean Object files

install:
	mv $(TARGET) ../bin
clean:
	rm -rf *.o
	-rm -rf $(TARGET)



