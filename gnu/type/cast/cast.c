#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<unistd.h>

#define LEN     512*8       //max 512个line at linesize=8;
#define STRID   512         //8个line.

int main()
{
    int i;
    char *arr;
    int psz = sysconf(_SC_PAGE_SIZE);
    arr = memalign(psz,LEN);

    for (i = 0; i < LEN; i+=STRID) {
        arr[i] = (long)&arr[i+STRID];
        printf("arr[i]:%#8x,arr[i+1]:%#8x,arr[i+2]:%#8x,&arr[i+%#8x]:%p\n",arr[i],arr[i+1],arr[i+2],STRID,&arr[i+STRID]);
    }
    arr[i-STRID] = (long)&arr[0];

    printf("-------------\n");

    /*
     *左值类型强制转
     */
    for (i = 0; i < LEN; i+=STRID) {
        *(char**)&arr[i] = (char*)&arr[i+STRID];
        printf("arr[i]:%#8x,arr[i+1]:%#8x,arr[i+2]:%#8x,&arr[i+%#8x]:%p\n",arr[i],arr[i+1],arr[i+2],STRID,&arr[i+STRID]);
    }
    *(char**)&arr[i-STRID] = (char*)&arr[0];

    free(arr);
    return 0;
}
