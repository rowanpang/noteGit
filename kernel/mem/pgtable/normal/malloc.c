#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc,char** argv)
{
        char *charp;
	char *ctmp;
        int sizeshift = 30;
	int skipshift = 3;

        char i = 0;

	if (argc == 2){
		sizeshift = atoi(argv[1]);
	} else if (argc == 3){
		sizeshift = atoi(argv[1]);
		skipshift = atoi(argv[2]);
	}

	printf("size shift:%d, skip shift:%d\n",sizeshift,skipshift);

        //charp = malloc( (unsigned long)1<<sizeshift );        OR
        charp = malloc( 1ul<<sizeshift );                       // 默认1为int, << 31后是负数,alloc 失败.
        ctmp = charp;

	printf("charp:%p, valued staring\n",charp);

        while ( ctmp < (charp + (1ul << sizeshift))) {
                *ctmp = i++;
                ctmp += 1<<skipshift;
		if (ctmp >= (charp + (1ul << sizeshift))){
			printf("now ctmp:%p\n",ctmp);
		}

        }

	printf("valued down\n");
        sleep(100);
        free(charp);

        return 0;
}
