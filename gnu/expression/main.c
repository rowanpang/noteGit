#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char **argv)
{
    int a = 2,b=3;

    int e = (a ?: b);		    //gnu extention for c. ref using-GCC_Using the GNU.... 6.7
    int c = (a==2 ?: b);
    int d = (a!=2 ?: b);

    printf("e:%d,c:%d,d:%d\n",e,c,d);

    return 0;
}
