#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>

static DEFINE_SPINLOCK(my_lock);
int i __attribute__ ((__unused__)) = 0; 

static int __init pmount_init(void)
{
    int ret = 0;
    DEFINE_MUTEX(my_mutex);
    spin_lock(&my_lock);
    mutex_unlock(&my_mutex);
    mutex_unlock(&my_mutex);
    printk("In func %s,mutexCount:%d\n",__func__,atomic_read(&my_mutex.count));

    spin_unlock(&my_lock);
    return ret;
}

static void __exit pmount_exit(void)
{
    printk("In func %s\n",__func__);

    printk("exit func %s\n",__func__);
}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("pmount test");
MODULE_LICENSE("GPL");

module_init(pmount_init);
module_exit(pmount_exit);
