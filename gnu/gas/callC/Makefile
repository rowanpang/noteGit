.PHONY:all clean 
all:msg.c main.s
	@gcc -g main.s msg.c -o msg.out
	@sed -i 's/call msg/call _msg/g' main.s
	@gcc -c -fleading-underscore msg.c -o msg-leading.o
	@gcc -g main.s msg-leading.o -o msg-leading.out
	@sed -i 's/call _msg/call msg/g' main.s
clean:
	@rm msg.out msg-leading.out msg-leading.o
