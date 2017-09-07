.PHONY:all
P=base pty thread

all: ${P}
%:%.c
	gcc -g $< -o $@ -lpthread

clean:
	@rm ${P}

