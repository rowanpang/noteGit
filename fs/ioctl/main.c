#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<linux/kd.h>
#include<linux/vt.h>

typedef _Bool bool;

int main(int argc,char **argv)
{
    int ret=0;
    int fd;
    int ttyX=1;
#define DEVPREFIX "/dev/tty"
    char devpath[20];
    int ledState;
    bool ttyMode;	//force to match str array.
    char *ttyModeStr[2]={
	"text",
	"graphics",
    };
    char ttyModeRevert=0;

    if (argc >1){
	sprintf(devpath,"%s%s",DEVPREFIX,argv[1]);
    }else{
	sprintf(devpath,"%s%s",DEVPREFIX,"1");
    }

    if (argc > 2){
	ttyModeRevert = 1;
    }

    
    printf("target dev is %s\n",devpath);

    ret = open(devpath,O_RDONLY);
    if (ret == -1) {
	perror("open target dev error");
	ret = errno;
	goto OUT;
    }
    fd = ret; 

    ret = ioctl(fd,KDGETLED,&ledState);
    if (ret == -1) {
	perror("error for KDGETLED");
	ret = errno;
	goto CLOSE;
    }
    printf("led state :%d\n",ledState);

    struct vt_mode vtMode;
    char *vtModeStr[3]={
	"auto",
	"process",
	"ackacq",
    };
    ret = ioctl(fd,VT_GETMODE,&vtMode);
    if (ret == -1) {
	perror("error for VT_GETMODE");
	ret = errno;
	goto CLOSE;
    }
    printf("cur vt->mode: %s\n",vtModeStr[vtMode.mode]);

    ret = ioctl(fd,KDGETMODE,&ttyMode);
    if (ret == -1) {
	perror("error for KDGETMODE");
	ret = errno;
	goto CLOSE;
    }
    printf("tty work cur mode: %s\n",ttyModeStr[ttyMode]);
    
    if (ttyModeRevert){
	ttyMode ^= 1;
	if (ttyMode == 0){
	    ret = ioctl(fd,VT_UNLOCKSWITCH);
            if (ret == -1) {
           	perror("error for KDSETMODE");
		ret = errno;
		goto CLOSE;
            }else{
		printf("vc text mode unlock switch");
	    }
	}

	ret = ioctl(fd,KDSETMODE,ttyMode);
	if (ret == -1) {
	    perror("error for KDSETMODE");
	    ret = errno;
	    goto CLOSE;
	}
	printf("tty work mode set to: %s\n",ttyModeStr[ttyMode]);
    }

CLOSE: 
    close(fd);
OUT:
    return ret;
}
