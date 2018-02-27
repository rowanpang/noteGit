#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main (int argc, char **argv)
{
    switch (argc) {
        case 2:
	    setvbuf(stdout,NULL,_IOLBF,0);
	    break;
        case 3:
	    setvbuf(stdout,NULL,_IONBF,0);
	    break;
        default:
	    break;
    }

    write(STDOUT_FILENO,"------write with \\n---\n",30);
    write(STDOUT_FILENO,"------write withou \\n--------",30);
    write(STDOUT_FILENO,"------write 2--------\n",30);

    printf ("-------print:line buffered out\\n----\n");
    fwrite("--------fwrite:stdout \\n------\n",40,1,stdout);
    printf ("-------print:without \\n,----");
    fwrite("--------fwrite:stdout 1------",40,1,stdout);

    fwrite("--------fwrite stderr 1------",40,1,stderr);
    fwrite("--------fwrite stderr \\n------\n",40,1,stderr);


    sleep(3);
    return 0;
}
