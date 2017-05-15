#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    char buf[10] = {0};
    int ret;
    int fd;
    int i;
    int flag=0;

    struct termios oldterm;
    
    if (argc > 2){
	flag = atoi(argv[2]);
    }

    fd = open(argv[1],O_RDWR);
    dprintf(STDOUT_FILENO,"fd is :%d\n",fd);

    ret = tcgetattr(fd,&oldterm);
    dprintf(STDOUT_FILENO,"fd:%d,term->c_lflag:%#x\n",fd,oldterm.c_lflag);
    ret = tcgetattr(STDIN_FILENO,&oldterm);
    dprintf(STDOUT_FILENO,"fd:%d,term->c_lflag:%#x\n",STDIN_FILENO,oldterm.c_lflag);

    if (flag){
	ret = tcsetattr(fd,TCSANOW,&oldterm);
	dprintf(STDOUT_FILENO,"fd:%d,term->c_lflag:%#x\n",fd,oldterm.c_lflag);
    }


    dprintf(STDOUT_FILENO,"start reading:");
    ret = read(fd,buf,sizeof(buf));
    printf("ret:%d,readin %d\n",ret,strlen(buf));

    for (i=0;i<ret;i++){
	printf("%#x ",buf[i]);
    }
    printf("\n------------\n");

    close(fd);
}

