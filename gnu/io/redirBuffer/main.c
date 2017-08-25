#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main (int argc, char **argv)
{

    write(STDOUT_FILENO,"------write with \\n---\n",30);
    write(STDOUT_FILENO,"------write 1--------",30);
    write(STDOUT_FILENO,"------write 2--------",30);

    printf ("-------print \\n----\n");
    fwrite("--------fwrite stdout \\n------\n",40,1,stdout);
    printf ("-------print ----");
    fwrite("--------fwrite stdout 1------",40,1,stdout);

    fwrite("--------fwrite stderr 1------",40,1,stderr);
    fwrite("--------fwrite stderr \\n------\n",40,1,stderr);


    sleep(10);
    return 0;
}
