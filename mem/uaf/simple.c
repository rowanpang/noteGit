#include <stdlib.h>
#include <mcheck.h>
#include <stdio.h>
#include <string.h>


struct rstring {
    char *strpredef;
    char *stralloc;
    int size;
};
int main(void) {

    int *a = NULL;
    struct rstring * rstr;
    char *hello = "Hello!";
    char *world;

    a = malloc(sizeof(int));    /* allocate memory and assign it to the pointer */
    free(a);                    /* we free the memory we allocated so we don't have leaks */
    *a = 10;

    rstr = malloc(sizeof(struct rstring));
    memset(rstr,0,sizeof(struct rstring));

    printf("------just after zalloc pre valued------\n");
    printf("size:%d,strpredef:%#p,stralloc:%#p\n",rstr->size, rstr->strpredef, rstr->stralloc);
    printf("rstr->strpredef:%s\n",rstr->strpredef);
    printf("rstr->stralloc:%s\n",rstr->stralloc);

    printf("------member valued------\n");
    rstr->size = 10;
    rstr->strpredef = hello;

    world = malloc(10);
    memcpy(world,hello,strlen(hello)+1);
    world[strlen(hello)] = '\0';
    rstr->stralloc = world;
    printf("size:%d,strpredef:%#p,stralloc:%#p\n",rstr->size, rstr->strpredef, rstr->stralloc);
    printf("rstr->strpredef:%s\n",rstr->strpredef);
    printf("rstr->stralloc:%s\n",rstr->stralloc);

    free(rstr);

    printf("-------After free(rstr)-------\n");
    printf("size:%d,strpredef:%#p,stralloc:%#p\n",rstr->size, rstr->strpredef, rstr->stralloc);
    printf("----before UAF -------\n");
    printf("rstr->strpredef:%s\n",rstr->strpredef);
    printf("rstr->stralloc:%s\n",rstr->stralloc);

    return 0;
}
