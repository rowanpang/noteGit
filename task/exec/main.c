#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc,char** argv)
{
	int ret;
	pid_t cpid;	
	int pipec2f[2];
	int pipef2c[2];
	
	pipe(pipec2f);
	pipe(pipef2c);

	cpid = fork();

	if(!cpid){
		/*child*/
		char *args[] = {"bash","-i",NULL};
		close(pipec2f[0]);
		close(pipef2c[1]);
		/*dup2(pipef2c[0],STDIN_FILENO);*/
		dup2(pipec2f[1],STDOUT_FILENO);
		dup2(pipec2f[1],STDERR_FILENO);
		sleep(10);
		/*ret = execv("/bin/bash",args);*/
		ret = execv("./hello",args);
		/*execv("/bin/bash",NULL);*/
		if(ret){
			perror("when execv");
		}
		printf("child prepare exit\n");
	}else if(cpid != -1){
		/*father*/
		int cstatus;
		char buf[1024];
		close(pipec2f[1]);
		close(pipef2c[0]);
		while(1){
			/*printf("father read:");*/
			ret = read(pipec2f[0],buf,1024);
			/*printf("%d bytes,%x",ret,buf[ret-1]);*/
			/*printf("father read:%d bytes \n%s",ret,buf);*/
			/*printf("out");*/
			printf("%s",buf);
			fflush(stdout);
			/*write(STDOUT_FILENO,buf,strlen(buf));*/
			/*printf("finish");*/
			bzero(buf,1024);
			if(!ret){
				break;
			}
		}
		wait(&cstatus);
		printf("child exit %d\n",cstatus);
	}else{
		perror("when fork");
		ret = -1;
		goto out;
	}
	
	ret = 0;
out:
	printf("father exit\n");
	return ret;
}
