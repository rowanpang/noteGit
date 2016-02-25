#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv)
{
    FILE* fp = fopen("bin_test.bin","w+");
    char bin[10] = {0};
    fwrite(bin,sizeof(bin),1,fp);
	
    return 0;
}

