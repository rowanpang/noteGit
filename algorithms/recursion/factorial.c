#include <stdio.h>
#include <stdlib.h>

int fac(int i)
{
    int ret;
    if(i==1)
        return 1;
    ret = i*fac(i-1);
    return ret;
}

int main(int argc,char** argv)
{

    printf("%d\n",fac(atoi(argv[1])));

    return 0;
}
