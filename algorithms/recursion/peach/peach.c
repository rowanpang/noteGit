#include <stdio.h>

int dayPeachs(int n,int day)
{
    int tmp,ret;

    tmp=(n+1)*2;
    printf("day:%d,num:%d\n",day,tmp);
    if(day>8)
        return tmp;

    ret = dayPeachs(tmp,(day+1));
    return ret;
}

int main(int argc,char** argv)
{

    printf("num:%d\n",dayPeachs(1,1));

    return 0;
}
