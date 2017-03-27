#include <stdio.h>
#include <string.h>
#include <errno.h> 
#include <fcntl.h> 
int main(void)
{
    int fd;
    extern int errno;
    if((fd =open("/dev/dsp",O_WRONLY)) < 0) {
        printf("errno=%d\n",errno);
        char * mesg = strerror(errno);
        printf("Mesg:%s\n",mesg); 
    }
    return 0;   
}
