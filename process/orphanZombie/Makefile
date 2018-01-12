.PHONY:all clean

TARGET = zombie orphan zombie-SIGCHLD forkMutil forkMutil-subreaper 	\
	 zombie-waitid zombie-asyncWait



all:$(TARGET)

%:%.c
	@gcc -g $^ -o $@

clean:
	@rm -rf $(TARGET)

