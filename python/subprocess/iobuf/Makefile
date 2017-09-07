.PHONY:all

all : test

%:%.c
	gcc -g $^ -o $@

test:app
	./popen.py
