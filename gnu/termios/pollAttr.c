#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int main(int argc,char **argv)
{
    struct termios orgtcattr,newtcattr;
    int ret;
    while(1){
	ret = tcgetattr(STDIN_FILENO,&orgtcattr);
	if (ret){
	    perror("when tcgetattr");
	    break;
	}

	printf("fd:0,term->c_lflag:%#x\n",orgtcattr.c_lflag);
	sleep(1);
    }

out:
    return ret;
}
	
	
