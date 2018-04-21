#include <stdio.h>

int map[100];

int main(int argc,char** argv)
{
    int i;
    map[2]=2;
    map[1]=1;

    for(i=2;i<10;i++){
        map[0]=map[1];
        map[1]=map[2];
        map[2]=map[2-1]+map[2-2];
        printf("%d ",map[i]);
    }
    return 0;
}
