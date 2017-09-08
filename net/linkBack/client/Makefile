.PHONY:all
P=base pty thread

all: ${P}
%:%.c
	gcc -g $< -o $@ -lpthread -lutil

clean:
	@rm ${P}

