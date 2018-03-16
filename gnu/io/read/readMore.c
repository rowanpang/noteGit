#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

void impossible()
{
    printf("Turn the impossible into possible!\n");
}

void vulnerable_function() {
    char buf[128];
    read(STDIN_FILENO, buf,512);
//    printf("after read %s",buf);
}

int main(int argc, char** argv) {
    char buf[100];
    write(STDOUT_FILENO, "Hello, World\n", 13);
    int ret;

    bzero(buf,100);
    while((ret = read(STDIN_FILENO,buf,sizeof(buf)/sizeof(char)))){
	if(!strncmp("q",buf,1)){
	    break;
	}else{
	    printf("ret:%d,readin %d bytes:%s\n",ret,strlen(buf),buf);
	}
	bzero(buf,100);
    }
}

