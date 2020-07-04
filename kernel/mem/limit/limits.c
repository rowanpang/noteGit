#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/vermagic.h>
#include <linux/slab.h>
#include <linux/stat.h>

#include <asm/unistd.h>

int orderlimits = MAX_ORDER ;
//module_param(orderlimits,int,S_IRWXU | S_IRWXG | S_IRWXO);
module_param(orderlimits,int,0644);

static int __init  plimit_init(void)
{
    int i;
    int size = PAGE_SIZE;
    char *p;
    printk("plimits test in func:%s\n",__func__);
    printk("page_size:%#lx,shift:%d, MAX_ORDER:%d, orderlimits:%d\n",PAGE_SIZE,PAGE_SHIFT,MAX_ORDER,orderlimits);

    for (i = 0; i < orderlimits; i++) {
	p = kmalloc(size<<i, GFP_KERNEL);
	if (!p){
	    printk("kmalloc error, size of page: %d\n",i);
	    goto out;
	}

	printk("kmalloc ok, size of page: %d\n",i);
	kfree(p);
    }



out:

    return 0;
}

static void __exit plimit_exit(void)
{
    printk("plimits test in func :%s\n",__func__);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("kmod plimit");
MODULE_AUTHOR("rowan pang");

module_init(plimit_init);
module_exit(plimit_exit);
