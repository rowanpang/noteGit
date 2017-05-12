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

    fd = open(argv[1],O_RDWR);
    dprintf(fd,"fd is :%d\n",fd);
    dprintf(STDOUT_FILENO,"fd is :%d\n",fd);

    write(STDOUT_FILENO, "Hello, World\n", 13);
    write(fd, "Hello, World\n", 13);
    ret = read(fd,buf,sizeof(buf));
    printf("ret:%d,readin %d\n",ret,strlen(buf));

    for (i=0;i<ret;i++){
	printf("%#x ",buf[i]);
	dprintf(fd,"%#x ",buf[i]);
    }
    printf("\n------------\n");
    dprintf(fd,"\n------------\n");

    sleep(120);

    close(fd);
}

