#include <stdio.h>

int map[100];

int main(int argc,char** argv)
{

    int i;
    map[0]=0;
    map[1]=1;
    for(i=2;i<10;i++){
        map[i]=map[i-1]+map[i-2];
        printf("%d ",map[i]);
    }
    return 0;
}
