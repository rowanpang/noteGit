#include <stdio.h>
#include <fcntl.h> /* O_ constants */
#include <unistd.h> /* ftruncate */
#include <sys/mman.h> /* mmap */

/*
 *ref same as this
 *    https://blog.csdn.net/wangjingyu00711/article/details/41774889
 */

int main()
{
    int fd;
    int *map;
    int size = sizeof(int);
    long pgSize= sysconf(_SC_PAGESIZE);

    char *name = "/a";

    printf("pagesize:%x\n",pgSize);

    shm_unlink(name);
    fd = shm_open(name, O_RDWR | O_CREAT, (mode_t)0600);
    /* THIS is the cause of the problem. */

    /*ftruncate(fd, size);*/	    //问题点,enable 运行正常
    /*Set the size of the shared memory object.  (A newly created shared memory object has a length of zero.)*/

    map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /* This is what generates the SIGBUS. */
    *map = 0;
}
