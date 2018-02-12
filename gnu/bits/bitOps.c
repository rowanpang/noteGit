#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i;

    if(argc > 1){
	i = strtol(argv[1],NULL,0);
    }else{
	i = 0x34804;
    }

    printf("%#x,find fist bit set is %d,ffz is %d\n", i,ffs(i),ffs(~i));


    return 0;
}

