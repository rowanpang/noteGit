#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void printDup(char *str)  
{  
    char *tmp = strdup(str);  
    printf("%s\n", tmp);  
    free(tmp);  
}  

int main(int argc,char **argv)  
{  
    char *ptr = "Hello World!";  
    printDup(ptr);  
    printf("Command to check memleak: valgrind --tool=memcheck --leak-check=full ./a.out\n");  

    char **nargv;
    int i;
    nargv = (char**)malloc(argc * sizeof(char *));
    for(i=1;i<argc;i++){
	nargv[i -1] = strdup(argv[i]);
    }
    nargv[i-1] = NULL;
    execvp(nargv[0],nargv);

    return 0;  
}  
