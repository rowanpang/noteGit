.PHONY:all clean
all:objdump.c
	@gcc -g -pg $< 
fentry:objdump.c
	@gcc -g -pg -mfentry $< -o $@
clean:
	@-rm -rf a.out fentry
