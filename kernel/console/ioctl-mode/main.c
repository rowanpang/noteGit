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

/*
 *usage:
 *    arg1,int X:if exit will open /dev/ttyX
 *    arg2,any  :if exit will revert the vc-mode for /dev/ttyX
 */

int main(int argc,char **argv)
{
    int ret=0;
    int fd;
    int ttyX=1;
#define DEVPREFIX "/dev/tty"
    char devpath[20];
    int ledState;
    bool vcMode;	//force to match str array.
    char *vcModeStr[2]={
	"text",
	"graphics",
    };
    char vcModeRevert=0;

    if (argc >1){
	sprintf(devpath,"%s%s",DEVPREFIX,argv[1]);
    }else{
	sprintf(devpath,"%s%s",DEVPREFIX,"");
    }

    if (argc > 2){
	vcModeRevert = 1;
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
    printf("cur vt->relsig: %d\n",vtMode.relsig);
    printf("cur vt->acqsig: %d\n",vtMode.acqsig);

    struct vt_stat vtState;
    ret = ioctl(fd,VT_GETSTATE,&vtState);
    if (ret == -1) {
	perror("error for KDGETMODE");
	ret = errno;
	goto CLOSE;
    }
    printf("cur active vt: %d\n",vtState.v_active);

    int kbdmode;
    char *kbdModeStr[]={
	"raw",
	"xlate",
	"mediumraw",
	"unicode",
	"off",
    };
    ret = ioctl(fd,KDGKBMODE,&kbdmode);
    if (ret == -1) {
	perror("error for KDGETMODE");
	ret = errno;
	goto CLOSE;
    }
    printf("cur kbdmod : %s\n",kbdModeStr[kbdmode%5]);

    ret = ioctl(fd,KDGETMODE,&vcMode);
    if (ret == -1) {
	perror("error for KDGETMODE");
	ret = errno;
	goto CLOSE;
    }
    printf("cur vcMode: %s\n",vcModeStr[vcMode]);
    
    if (vcModeRevert){
	vcMode ^= 1;
	if (vcMode == 0){
	    struct vt_mode nvtm = vtMode;
	    nvtm.mode = VT_AUTO;
	    nvtm.relsig = 0;
	    nvtm.acqsig = 0;
	    ret = ioctl(fd,VT_SETMODE,&nvtm);
	    if (ret == -1) {
		perror("error for VT_SETMODE");
		ret = errno;
		goto CLOSE;
	    }

	    ret = ioctl(fd,VT_UNLOCKSWITCH);
            if (ret == -1) {
           	perror("error for KDSETMODE");
		ret = errno;
		goto CLOSE;
            }else{
		printf("vc text mode unlock switch");
	    }
	}

	ret = ioctl(fd,KDSETMODE,vcMode);
	if (ret == -1) {
	    perror("error for KDSETMODE");
	    ret = errno;
	    goto CLOSE;
	}
	printf("tty work mode set to: %s\n",vcModeStr[vcMode]);
    }

CLOSE: 
    close(fd);
OUT:
    return ret;
}
