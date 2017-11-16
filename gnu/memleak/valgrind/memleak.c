#include<stdio.h>
#include<stdlib.h>
void f(void) 
{ 
    int* x = malloc(10 * sizeof(int)); 
    x[9] = 2; 
    x[10] = 0; 	//problem 1: heap block overrun 
    //free(x);	//problem 2: memory leak -- x not freed 
}

int main(void) 
{ 
    int i = 2; 
    /*int i ; */
    f(); 
    printf("i=%d/n",i); 	//problem 3: use uninitialised value. 
    return 0; 
} 
