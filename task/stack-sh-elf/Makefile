.PHONY:all clean
all:execve myecho 

execve:execve.c
	@gcc -g $< -o $@
myecho:myecho.c
	@gcc -g $< -o $@

clean:
	@rm execve myecho
