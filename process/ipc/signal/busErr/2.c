#include <stdio.h>
#include <fcntl.h> /* O_ constants */
#include <unistd.h> /* ftruncate */
#include <sys/mman.h> /* mmap */
#include <sys/stat.h> /* mmap */
#include <errno.h> /* ftruncate */
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
    char buf[128];
    char c = 0;

    printf("pagesize:%x\n",pgSize);

retry:
    fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, (mode_t)0600);
    if (fd == -1 ){
        if (errno == EEXIST) {
            sprintf(buf,"file /dev/shm/%s exist, confirm ,enter to delete",name);
            perror(buf);

            c = getchar();
            if (c !='\n'){
                return 0;
            } else {
                printf("unlink file\n");
                shm_unlink(name);
                goto retry;
            }
        } else {
            return -1;
        }
    }
    /* THIS is the cause of the problem. */

    ftruncate(fd, size);	                //问题点,enable 运行正常
    /*Set the size of the shared memory object.  (A newly created shared memory object has a length of zero.)*/

    map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /* This is what generates the SIGBUS. */
    *map = 0;
    shm_unlink(name);
}
