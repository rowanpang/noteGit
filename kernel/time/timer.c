#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>

struct timer_list timer;

void timer_func(int data)
{
	printk(KERN_DEBUG "timer func\n");
}

static int __init  ptimer_init(void)
{
	printk("in func :%s\n",__func__);
	init_timer(&timer);
	timer.data = 10;
	timer.expires = jiffies + (10*HZ);
	timer.function = timer_func;
	add_timer(&timer);

    return 0;
}

static void __exit ptimer_exit(void)
{
	del_timer(&timer);

	printk("exit in func :%s\n",__func__);
}

MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("ptimer test"); 
MODULE_AUTHOR("rowan pang"); 

module_init(ptimer_init); 
module_exit(ptimer_exit);
