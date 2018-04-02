#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>

static int __init  timeTest_init(void)
{
    printk("in func :%s\n",__func__);
    printk("cpu_clock(0):%lld ns\n",cpu_clock(0));
    printk("cpu_clock(1):%lld ns\n",cpu_clock(1));
    printk("local_clock():%lld ns\n",local_clock());

    return 0;
}

static void __exit timeTest_exit(void)
{
    printk("exit in func :%s\n",__func__);
}

MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("time release  test"); 
MODULE_AUTHOR("rowan pang"); 

module_init(timeTest_init); 
module_exit(timeTest_exit);
