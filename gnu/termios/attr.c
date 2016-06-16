#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int main(int argc,char **argv)
{
	struct termios orgtcattr,newtcattr;
	int ret;
	if(ret = tcgetattr(STDIN_FILENO,&orgtcattr)){
		perror("when tcgetattr");
		goto out;
	}

	newtcattr = orgtcattr;
	newtcattr.c_lflag &=  (~ECHO);
	if(ret = tcsetattr(STDIN_FILENO,TCSAFLUSH,&newtcattr)){
		perror("when tcgetattr");
		goto out;
	}

	char buf[10];
	ret = read(STDIN_FILENO,buf,4);
	printf("read %d,%s",ret,buf);

	ret = 0;
	tcsetattr(STDIN_FILENO,TCSAFLUSH,&orgtcattr);
out:
	return ret;
}
	
	
