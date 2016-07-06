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
//	printf("after read %s",buf);
}

int main(int argc, char** argv) {
	char buf[100] = {0};
	write(STDOUT_FILENO, "Hello, World\n", 13);
	int ret;
	/*
	 *while((ret = read(STDIN_FILENO,buf,99))){
	 *    if(!strncmp("q",buf,1)){
	 *        break;
	 *    }else{
	 *        printf("ret:%d,readin %d bytes:%s\n",ret,strlen(buf),buf);
	 *    }
	 *    bzero(buf,100);
	 *}
	 */

	printf("flush test start\n");
	#define READ_SIZE 4
	while((ret = read(STDIN_FILENO,buf,READ_SIZE))){
		if(!strncmp("q",buf,1)){
			break;
		}else{
			printf("ret:%d,readin %d bytes:%s",ret,strlen(buf),buf);
		}

		char c;
		if((ret == READ_SIZE) && (buf[READ_SIZE - 1] != '\n')){
			printf("flush\n");
			tcflush(STDIN_FILENO,TCIFLUSH);		//仅对terminal有效.
			/*while ((c=getchar()) != '\n' && c != EOF);*/
		}
		bzero(buf,100);
	}

	vulnerable_function();
}

