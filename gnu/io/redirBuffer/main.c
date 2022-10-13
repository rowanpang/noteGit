#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main (int argc, char **argv)
{
    printf("glibc BUFSIZ:%d\n",BUFSIZ);
    fflush(stdout);
    switch (argc) {
        case 2:
            printf("glibc set _IOLBF");
            fflush(stdout);
	    setvbuf(stdout,NULL,_IOLBF,0);
	    break;
        case 3:
            printf("glibc set _IONBF");
	    setvbuf(stdout,NULL,_IONBF,0);
	    break;
        default:
	    break;
    }

    write(STDOUT_FILENO,"---------write syscall func---\n",20);

    write(STDOUT_FILENO,"---------write with \\n---\n",20);
    write(STDOUT_FILENO,"---------write withou \\n--------",20);
    write(STDOUT_FILENO,"---------write 2--------\n",20);

    write(STDOUT_FILENO,"---------glibc APIs-----\n",20);
    printf("printf:line buffered out\\n----\n");
    fwrite("fwrite:stdout \\n------\n",20,1,stdout);
    printf("printf:without \\n,----");
    fwrite("fwrite:stdout 1------",20,1,stdout);

    fwrite("fwrite stderr 1------",20,1,stderr);
    fwrite("fwrite stderr \\n------\n",20,1,stderr);


    sleep(3);
    fflush(stdout);
    return 0;
}
