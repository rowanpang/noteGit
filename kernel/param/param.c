#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sem.h>
#include <linux/list.h>

int count = 0;
char *who = NULL;

module_param(count,int,0700);
module_param(who,charp,0700);

static int __init  traverse_init(void)
{
    printk(KERN_INFO "The number of process is:%d,who:%s\n",count,who);

    return 0;
}

static void __exit traverse_exit(void)
{
      printk(KERN_INFO "traverse hello world exit\n");
}
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("traverse task list"); 
MODULE_AUTHOR("Zou Nan hai"); 

module_init(traverse_init); 
module_exit(traverse_exit);
