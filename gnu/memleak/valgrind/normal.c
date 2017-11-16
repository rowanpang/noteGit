#include<stdio.h>
#include<stdlib.h>
void f(void) 
{ 
    int* x = malloc(10 * sizeof(int)); 
    x[9] = 2; 
    free(x);	
}

int main(void) 
{ 
    int i = 2; 
    f(); 
    printf("i=%d/n",i); 
    return 0; 
} 
