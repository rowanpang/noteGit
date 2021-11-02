#include <stdlib.h>
#include <mcheck.h>

int main(void) {

    int* a = NULL;

    a = malloc(sizeof(int));    /* allocate memory and assign it to the pointer */

    free(a);                    /* we free the memory we allocated so we don't have leaks */
    return 0;
}
