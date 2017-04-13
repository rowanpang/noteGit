#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <error.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

int main(int argc,char **argv)
{
    int ret;
    int master;
    char *slavePath;
    int slave;
    ret = open("/dev/ptmx",O_RDWR|O_NOCTTY);	//equal to getpt()
    if(ret == -1){
	perror("ptmx open error");
	goto OUT;
    }
    master = ret;

    ret = grantpt(master);
    if(ret != 0){
	perror("grantpt error!");
	goto CLOSE_MASTER;
    }
    ret = unlockpt(master);
    if(ret != 0){
	perror("unlockpt error!");
	goto CLOSE_MASTER;
    }

    slavePath = ptsname(master);    
    printf("new ptmx paire: %d,%s\n",master,slavePath);
    ret = open(slavePath,O_RDWR);
    if(ret == -1){
	perror("slave open error");
	goto CLOSE_MASTER;
    }
    slave = ret;
    printf("new ptmx paire: %d,%d\n",master,slave);

    ret = fork();
    if(ret != 0){
	/*father*/
	int childPId = ret;
	char buf[256];
	fd_set fd_in;
	int infd;
	int outfd;

	close(slave);
	while(1){
	AGAIN:
	    FD_ZERO(&fd_in);
	    FD_SET(STDIN_FILENO,&fd_in);	//0
	    FD_SET(master,&fd_in);

	    ret = select(master+1,&fd_in,NULL,NULL,NULL);
	    if(ret == -1){
		perror("select error");
		goto WAIT_CHILD;
	    }
	    if(FD_ISSET(STDIN_FILENO,&fd_in)){
		infd = STDIN_FILENO;
		outfd = master;
		dprintf(2,"read from stdin\n");
	    }else if(FD_ISSET(master,&fd_in)){
		infd = master;
		outfd = STDOUT_FILENO;
		dprintf(2,"read from ptx-master\n");
	    }

	    bzero(buf,sizeof(buf));
	    ret = read(infd,buf,sizeof(buf));
	    dprintf(2,"read in:%s\n",buf);
	    if(ret < 0){
		perror("read in error");
		goto AGAIN;
	    }else if(ret == 0){
		break;
	    }
	    ret = write(outfd,buf,ret);	    //send stdin to master.
	    if(ret == 0){
		perror("write out error");
	    }
	}

WAIT_CHILD:
	waitpid(childPId,&ret,0);
	close(master);
	goto OUT;
    }else{
	/*child*/
	struct termios oterm;
	struct termios nterm;
	char **nargv;
	int i;

	close(master);
	ret = tcgetattr(slave,&oterm);
	nterm = oterm;
	cfmakeraw(&nterm);
	tcsetattr(slave,TCSANOW,&nterm);

	close(0);
	close(1);
	close(2);

	dup(slave);	    //pty to be the stdin/out/err
	dup(slave);
	dup(slave);

	close(slave);
	setsid();
	ioctl(0,TIOCSCTTY,1);

	nargv = (char**)malloc(argc * sizeof(char*));
	for (i=1;i<argc;i++){
	    nargv[i-1] = strdup(argv[i]);
	}
	nargv[i-1] = NULL;

	execvp(nargv[0],nargv);
    }

    ret = 0;
CLOSE_SLAVE:
    close(slave);
CLOSE_MASTER:
    close(master);
OUT:
    return ret;
}
