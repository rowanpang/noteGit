#include <stdio.h>

/*
 *case sizeof(long) == sizeof(void*) for both x86_64 and i386
 *so use it as the inter type when do cast
 */

static inline long PTR_ERR(const void *ptr)
{
    return (long) ptr;
}

static inline void * ERR_PTR(long error)
{
    return (void *) error;
}


int main()
{
    int ret = -2;
    int *p = &ret;

    printf("err to ptr:%p\n",ERR_PTR(ret));
    printf("ptr to err:%d\n",PTR_ERR(p));

}
