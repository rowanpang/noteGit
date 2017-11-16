#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printDup(char *str)  
{  
    char *tmp = strdup(str);  
    printf("%s\n", tmp);  
    /*free(tmp);  */
}  

int main()  
{  
    char *ptr = "Hello World!";  
    printDup(ptr);  
    printf("Command to check memleak: valgrind --tool=memcheck--leak- check=yes ./a.out\n");  
    return 0;  
}  
