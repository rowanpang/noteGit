.PHONY:all
all:main.c
	@gcc -g main.c -o a.out
	@gcc -g main.c -o a.out.fortify -Wp,-D_FORTIFY_SOURCE=2 -O2

.PHONY:clean
clean:
	@rm -rf a.out*
