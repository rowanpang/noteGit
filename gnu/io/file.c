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

    fd = open(argv[1],O_RDONLY);

    write(STDOUT_FILENO, "Hello, World\n", 13);
    ret = read(fd,buf,sizeof(buf));
    printf("ret:%d,readin %d\n",ret,strlen(buf));

    for (i=0;i<ret;i++){
	printf("%#x ",buf[i]);
    }
    printf("\n------------\n");

    close(fd);
}

