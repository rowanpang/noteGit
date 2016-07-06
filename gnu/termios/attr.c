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
	/*newtcattr.c_lflag &=  (~(ECHO | ICANON));*/
	newtcattr.c_lflag &=  (~(ICANON));
	if(ret = tcsetattr(STDIN_FILENO,TCSAFLUSH,&newtcattr)){
		perror("when tcgetattr");
		goto out;
	}

	char buf[10];
	while(1){
		ret = read(STDIN_FILENO,buf,4);
		printf("read %d,%s",ret,buf);
		fflush(stdout);
		if(buf[0] == 'q'){
			break;
		}
	}

	ret = 0;
	tcsetattr(STDIN_FILENO,TCSAFLUSH,&orgtcattr);
out:
	return ret;
}
	
	
