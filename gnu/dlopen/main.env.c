#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include <dlfcn.h>
#include <sys/types.h>

int main(int argc,char** argv)
{
    int ret;
    char* msg= "askdf";

    void *dlh;
    int (*showmsg)(char* msg);
    char *error;

    char cpath[PATH_MAX]={};
    getcwd(cpath,PATH_MAX);
    strcat(cpath,"/ldpathEnv");
    printf("cpath: %s\n",cpath);

    ret = setenv("LD_LIBRARY_PATH",cpath,1);
    if (ret){
	perror("when setenv");
	return -1;
    }

    printf("pid: %d\n",getpid());

    system("cat /proc/self/environ");
    printf("\n\n");

    dlh = dlmopen(LM_ID_BASE,"msg.so",RTLD_LAZY);
    /*dlh = dlopen("msg.so",RTLD_LAZY);*/
    if (!dlh){
	printf("dlopen error: ");
	goto ERROUT;
    }
    showmsg = dlsym(dlh,"showmsg");
    if (!showmsg){
	printf("dlsym showmsg: ");
	goto ERROUT;
    }


    showmsg("hello world");
    dlclose(dlh);

ERROUT:
    error = dlerror();
    if (error != NULL) {
	fprintf(stderr, "%s\n", error);
	return -1;
    }

    return 0;
}
