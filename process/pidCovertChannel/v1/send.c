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

    about();
    synchronize(1);
    while(1){
	send(1);
    }

    ret = 0;
    return ret;
}
