#include <stdio.h>
#include <stdlib.h>

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

struct stpp {
    void **vpp1;
    void **vpp2;
};

int main()
{
    int ret = -2;
    int ret2 = -3;
    int *p = &ret;
    int *p2 = &ret2;
    int **pp = &p;
    int **pp2 = &p2;
    struct stpp *vpp;
    vpp = malloc(sizeof(struct stpp));
    vpp->vpp2 = (void**)&p;
    vpp->vpp1 = (void**)pp2;
    *(vpp->vpp2) = vpp->vpp1;

    printf("ret:%p\n",&ret);
    printf("ret2:%p\n",&ret2);
    printf("vtpp:%p\n",vpp);
    printf("p:%p\n",p);
    printf("&p:%p\n",&p);
    printf("*p:%p\n",*p);
    printf("vpp1:%p\n",vpp->vpp1);
    printf("vpp2:%p\n",vpp->vpp2);
    printf("err to ptr:%p\n",ERR_PTR(ret));
    printf("ptr to err:%d\n",PTR_ERR(p));

}
