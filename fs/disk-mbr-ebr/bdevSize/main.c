#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<linux/kd.h>
#include<linux/vt.h>
#include<linux/fs.h>

typedef _Bool bool;

int main(int argc,char **argv)
{
    int fd;
    //off_t size
    uint64_t size;
    int len;
    int r;

    if ((fd = open("/dev/sda", O_RDONLY)) < 0) {
        printf("open error %d\n", errno);
        exit(-1);
    }

#if 0
    //会报错
    if ((size = lseek(fd, 0, SEEK_END)) < 0)
    {
        printf("lseek errno %d\n", errno);
        exit(-1);
    }
#endif
    if ((r = ioctl(fd, BLKGETSIZE64, &size)) < 0) {
        printf("ioctl error %d\n", errno);
        exit(-1);
    }

    printf("size of sda = %llu bytes\n", size);

    return 0;
}
