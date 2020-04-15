#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/vermagic.h>
#include <asm/unistd.h>

static unsigned long **aquire_sys_call_table(void)
{
    unsigned long **entry = (unsigned long **)PAGE_OFFSET;

    for (;(unsigned long)entry < ULONG_MAX; entry += 1) {
	if (entry[__NR_close] == (unsigned long *)sys_close) {
	    pr_info("found sys_call_table value :%p\n",entry);
	    return entry;
	}
    }

    return NULL;
}

static int __init  systbl_init(void)
{
    printk("in func :%s\n",__func__);
    pr_info("kernel vermagic:%s\n",VERMAGIC_STRING);

    /*pr_info("sys_call_table sysmbol value :%p\n",sys_call_table);*/


    aquire_sys_call_table();



    return 0;
}

static void __exit systbl_exit(void)
{
    printk("exit in func :%s\n",__func__);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("kmod systbl");
MODULE_AUTHOR("rowan pang");

module_init(systbl_init);
module_exit(systbl_exit);
