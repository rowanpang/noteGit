#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <errno.h>
#include <regex.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "lib.h"

int main(int argc, char *argv[])
{
    int ret;
    int cnt = 0;

    about();
    synchronize(1);
    while(1){
	if(cnt & 0x01){
	    send(1);
	}else{
	    send(0);
	}
	//if(cnt) break;
	cnt++;
    }

    ret = 0;
    return ret;
}
