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

#include "lib.h"

int main(int argc, char *argv[])
{
    int ret;
    int val;

    about();
    synchronize(0);
    while(1){
	val = recive();
    }

    ret = val;
    ret = 0;
    return ret;
}
