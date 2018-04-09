#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int ret;
    pid_t cpid;

    char *chfn_argv[]={
	"/usr/bin/chfn",
	NULL
    };

    char *pkexec_argv[] = {
	"/usr/bin/pkexec",
	"/bin/sh",
	NULL
    };

    char buf[64];


    cpid = fork();

    if(cpid<0){
	perror("when fork");
	ret = -1;
	goto out;
    }else if(cpid){
	/*parent*/
	waitpid(cpid,NULL,0);
	/*close(0);close(1);close(2);*/
	/*sleep(50000);*/
	/*execv(chfn_argv[0],chfn_argv);*/
	printf("child pid:%d,tcpgrp:%d\n",cpid,tcgetpgrp(0) );

    }else{
	/*child*/
	/*setsid();*/
	close(0);
	/*close(1);*/
	/*close(2);*/
	/*execv(chfn_argv[0],chfn_argv);*/
	/*execv(pkexec_argv[0],pkexec_argv);*/
	/*printf("tcpgrp:%d\n",tcgetpgrp(0) );*/
	sleep(2);
	printf("tcpgrp:%d\n",tcgetpgrp(0) );
	/*read 0,有个窗口期,如果不加sleep 直接read,则不会报EIO.*/
	ret = read(0,buf,20);
	if (ret == -1){
	    perror("when read");
	    goto out;
	}else{
	    printf("read %d byts\n",ret);
	}

    }



    ret = 0;
out:

    printf("process exit pid:%d,pgid:%d\n",getpid(),getpgrp() );
    return ret;
}
