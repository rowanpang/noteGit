#include<stdio.h>

int main()
{
    int u=0;
    int t=0;
    int u1=0;
    char *p=(char*)&t+1;
    printf("%#x\n", *(int*)p);
    __sync_val_compare_and_swap((int*)p,0x9d000000,1);

    printf("%p\n",p);
    printf("%#x\n",t);
}
