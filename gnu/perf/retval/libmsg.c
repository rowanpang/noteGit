#include <stdio.h>
#include <string.h>

int mymsg(const char* msg,int cnt)
{
    int i = 9;
    printf("in func:%s, msg:%s, cnt:%d\n",__func__,msg,cnt);
    return cnt;
}
