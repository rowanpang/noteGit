#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_SERVER "/tmp/fifoserver"
int handle_client(char* arg)
{
    int ret;
    ret = open(arg,O_WRONLY|O_NONBLOCK,0);
    if(ret == -1){
	switch(errno){
	    case ENXIO:
		printf("open %s error\n",arg);
		printf("no process has the fifo open for reading\n");
		break;
	    default:
		printf("something wrong with open the fifo except for ENXIO");
		break;
	}
    }else if(ret > 0) {
	printf("open %s ok\n",arg);
    }
    unlink(arg);

    return ret;
}

int main(int argc,char** argv)
{
    int ret;
    int r_rd;
    int w_fd;
    pid_t pid;

    ret = mkfifo(FIFO_SERVER,O_CREAT|O_EXCL);
    if(ret <0){
        perror("cannot create fifoserver\n");
    }
    handle_client(FIFO_SERVER);

    return 0;
}

