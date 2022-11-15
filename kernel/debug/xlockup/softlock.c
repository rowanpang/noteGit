#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/kthread.h>


static int delays = 30;
module_param(delays,int,0700);

static int __init softlock_init(void)
{
    int ret = 0;
    unsigned long delay=jiffies+delays*HZ;	    //delay 30s 

    printk("In func %s,delay:%ds\n",__func__,delays);
    while(time_before(jiffies,delay));

    return ret;
}

static void __exit softlock_exit(void)
{ 
    printk("In func %s\n",__func__); 
}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("softlock test");
MODULE_LICENSE("GPL");

module_init(softlock_init);
module_exit(softlock_exit);
