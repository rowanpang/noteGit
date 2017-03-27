#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc,char **argv)
{
    int ret=0;
    char *exfile=NULL;
#define PATH "/bin/"
    if(argc < 2){
	printf("need args at least one\n");
	ret = 1;
	goto OUT;
    }
    if(*argv[1] == '/'){
	exfile=argv[1];
    }else{
	exfile=malloc(256);
	sprintf(exfile,"%s%s",PATH,argv[1]);
    }
    /*execv(exfile,argv+1);*/
    int pid = fork();
    if(pid == 0){
	/*setuid(0);*/
	execv(exfile,argv+1);
    }else{
	waitpid(pid,NULL,0);
    }

OUT:
    if(*argv[1] != '/'){
	free(exfile);
    }
    return ret;
}
