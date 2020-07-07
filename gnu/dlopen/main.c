#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <dlfcn.h>

int main(int argc,char** argv)
{
    int ret;
    char* msg= "askdf";

    void *dlh;
    int (*showmsg)(char* msg);
    char *error;

    dlh = dlopen("msg.so",RTLD_LAZY);
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
