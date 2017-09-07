.PHONY:all
all:client listener

%:%.c
	@gcc -g $< -o $@ -lpthread

clean:
	@rm client listener 

