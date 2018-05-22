#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>


#define FIFO_SERVER "/tmp/fifoserver"
#define SIZE (1024*8)

int main(int argc,char** argv)
{
    int ret;
    int fd;
    char w_buf[SIZE];
    int real_wnum;
    int total = 0;
    memset(w_buf,0,SIZE);

    ret = mkfifo(FIFO_SERVER,O_CREAT|O_EXCL);
    if (ret < 0){
        if(errno != EEXIST){
            printf("cannot create fifoserver\n");
        }else{
            printf("exist\n");
        }
    }

AGAIN:
    fd=open(FIFO_SERVER,O_WRONLY|O_NONBLOCK,0);
    /*
     *fd=open(FIFO_SERVER,O_WRONLY,0);
     *block and non-block
     */
    if(fd == -1){
        if(errno==ENXIO){
            perror("no reading process,again\n");
            sleep(1);
            goto AGAIN;
        }
        perror("open error exit");

        ret = -1;
        goto OUT;
    }
    while(1){
WAGAIN:
        /*real_wnum=write(fd,w_buf,2048);*/
        real_wnum=write(fd,w_buf,2048*4);
        if(real_wnum == -1){
            if(errno==EAGAIN){
                printf("write to fifo error; try later\n");
                sleep(1);
                goto WAGAIN;
            }
        }
        total += real_wnum;

        printf("real write num is %d,total:%d\n",real_wnum,total);
        usleep(1000*100);
    }

    ret = 0;
OUT:
    return ret;
}
