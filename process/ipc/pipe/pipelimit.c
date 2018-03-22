/*this file work for test the pipe write*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc,char* argv[])
{
    int pipe_fd[2];
    pid_t pid;
    char r_buf[4];
    int writenum;
    int cmd;
    int ret;
    char* w_buf;
    int size;

    memset(r_buf,0,sizeof(r_buf));
    /*block and O_NONBLOCK test*/
    /*if(pipe(pipe_fd)<0){*/
    if(pipe2(pipe_fd,O_NONBLOCK)<0){
	printf("pipe create error\n");
	return -1;
    }

    if((pid=fork())==0){
	/*
	 *close(pipe_fd[0]);
	 *If all file descriptors referring to the read end of a pipe have been
	 *closed, then a write(2) will cause a SIGPIPE signal to be  generated
	 */

	close(pipe_fd[1]);
	close(pipe_fd[0]);
	sleep(10);
	ret = 0;
    }else if(pid > 0){
	sleep(1);  //等待子进程完成关闭读端的操作
	printf("total num:%d\n",writenum);
	/*close(pipe_fd[0]);//write*/
	writenum = 0;
	while(1){
	    w_buf = "0123456789abcdef";
	    size = 1024*16;
	    w_buf = malloc(size);
	    ret = write(pipe_fd[1],w_buf,size);
	    if(ret <= 0){
		perror("when write to pipe");
		break;
	    }
	    writenum += ret;
	    printf("write:%d,total num:%d\n",ret,writenum);
	}
	printf("break out,total num:%d\n",writenum);

	close(pipe_fd[1]);

	waitpid(pid,NULL,0);
    }

OUT:
    return ret;
}
