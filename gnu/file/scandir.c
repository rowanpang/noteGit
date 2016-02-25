#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

int  main(void)
{
    struct dirent **namelist;
    int n;

    n = scandir(".", &namelist, 0, alphasort);
    if (n < 0)
        perror("scandir");
    else {
        while (n--) {
            printf("name:%s, mode:%d \n", namelist[n]->d_name,namelist[n]->d_type);
            free(namelist[n]);
        }
        free(namelist);
    }
}
