#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <sys/epoll.h>
#include <sys/socket.h>
/*canary 金丝雀*/
int main(int argc, char *argv[])
{
    static int canary = 0;
    printf(argv[1]);
    printf("\n");
    /*printf("Canary at 0x%08lx = 0x%08x\n", (long)&canary, canary);*/

    return 0;
}
