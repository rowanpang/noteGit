#include<stdio.h>
/*
 *ref:
 *    https://www.linuxquestions.org/questions/programming-9/bus-error-core-dumped-due-to-sigbus-signal-340567/
 */

int main()
{
    short a;

    scanf("%d",&a);
    printf("\n a= %d",a);

    return 0;
}
