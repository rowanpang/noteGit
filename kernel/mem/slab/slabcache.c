#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/kthread.h>


#define SLAB_TEST_MAXNUM 10
struct slab_test {
    union {
        char arr[64];
        char *name;
    };
};

struct kmem_cache *testkcache = NULL;
struct slab_test* pslabobj[SLAB_TEST_MAXNUM] = {NULL};


static int __init slabcache_test_init(void)
{
    int ret = 0;
    int i = 0;
    struct slab_test* tmp;
    printk("In func %s\n",__func__);

    testkcache = kmem_cache_create("slabcache test cachep",sizeof(struct slab_test),0,SLAB_HWCACHE_ALIGN|SLAB_PANIC, NULL);
    if (!testkcache)
        return -ENOMEM;

    for(i=0; i<SLAB_TEST_MAXNUM; i++){
        tmp = kmem_cache_alloc(testkcache,GFP_KERNEL);
        if (!tmp){
            ret = -ENOMEM;
            goto memerr;
        }
        pslabobj[i] = tmp;

        printk("allocated one obj from testkcache,total:%d\n",i);
    }

    return ret;

memerr:
    for(i=0; i<SLAB_TEST_MAXNUM; i++){
        if (pslabobj[i]){
            kmem_cache_free(testkcache,pslabobj[i]);
            printk("freed one obj idx:%d\n",i);
        }
    }

    return ret;
}

static void __exit slabcache_test_exit(void)
{
    int i=0;
    printk("In func %s\n",__func__);
    for(i=0; i<SLAB_TEST_MAXNUM; i++){
        if (pslabobj[i]){
            kmem_cache_free(testkcache,pslabobj[i]);
            printk("freed one obj idx:%d\n",i);
        }
    }

    if(testkcache){
        kmem_cache_destroy(testkcache);
        printk("destroyed testkcache");
    }

}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("slabcache test");
MODULE_LICENSE("GPL");

module_init(slabcache_test_init);
module_exit(slabcache_test_exit);
