#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int __init  ptimer_init(void)
{
	printk("in func :%s\n",__func__);
    return 0;
}

static void __exit ptimer_exit(void)
{
	printk("exit in func :%s\n",__func__);
}

MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("ptimer test"); 
MODULE_AUTHOR("rowan pang"); 

module_init(ptimer_init); 
module_exit(ptimer_exit);
