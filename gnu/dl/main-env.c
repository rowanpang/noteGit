#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <dlfcn.h>
#include <sys/types.h>

int main(int argc,char** argv)
{
    int ret;
    char* msg= "askdf";

    void *dlh;
    int (*showmsg)(char* msg);
    char *error;

    ret = setenv("LD_LIBRARY_PATH","/home/pangwz/noteGit/gnu/dl/ldpathEnv",1);
    if (ret){
	perror("when setenv");
	return -1;
    }

    printf("pid: %d\n",getpid());
    /*system("cat /proc/self/environ");*/

    dlh = dlmopen(LM_ID_BASE,"msg.so",RTLD_LAZY);
    showmsg = dlsym(dlh,"showmsg");

    error = dlerror();
    if (error != NULL) {
	fprintf(stderr, "%s\n", error);
	return -1;
    }

    showmsg("hello world");
    dlclose(dlh);
    
    return 0;
}
