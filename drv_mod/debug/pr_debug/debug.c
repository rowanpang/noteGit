#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sem.h>
#include <linux/list.h>

static int __init  traverse_init(void)
{
    struct task_struct *pos;
    struct list_head *current_head;
    int count=0;
    printk(KERN_INFO "info Traversal module is working..\n");
    printk(KERN_DEBUG "debug Traversal module is working..\n");

    pr_debug(KERN_INFO "pr_debug info Traversal module is working..\n");
    pr_debug(KERN_DEBUG "pr_debug debug Traversal module is working..\n");
    return 0;
}

static void __exit traverse_exit(void)
{
      printk(KERN_INFO "info traverse hello world exit\n");
      printk(KERN_DEBUG "debug traverse hello world exit\n");

      pr_debug(KERN_INFO "pr_debug info traverse hello world exit\n");
      pr_debug(KERN_DEBUG "pr_debug debug traverse hello world exit\n");
}

MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("debug test"); 
MODULE_AUTHOR("pangwz"); 

module_init(traverse_init); 
module_exit(traverse_exit);
