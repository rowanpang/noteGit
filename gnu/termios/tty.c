#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc,char** argv)
{
	int ret;
	int ptty;
	int sid;
	ptty = open("/dev/tty",O_RDWR);
	printf("tty name:%s\n",ttyname(ptty));

	sid = getsid(0);
	if(sid == -1){
		perror("when getsid");
		ret = errno;	
		goto out;
	}else{
		printf("got sid %d\n",sid);
	}
	
	printf("got pid %d\n",getpid());
	/*
	 *in case we are group leader do fork;after fork,
	 *    the pgrp of child is equal to the parent pgrp
	 *    parent pgrp is parent itself
	 */
	if(getpgrp() == getpid()){
		switch(fork()){
			case -1:
				perror("when fork");
				break;
			case 0:		//child
				break;
			default:
				exit(0);	//parent
				break;
		}
	}
	/*sid = setsid();*/
	/*if(sid == -1){*/
		/*perror("when setsid");*/
		/*ret = errno;	*/
		/*goto out;*/
	/*}*/

	sleep(45);
out:
	return ret;
}
