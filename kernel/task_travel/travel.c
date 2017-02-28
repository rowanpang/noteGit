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
//    printk(KERN_EMERG"init traverse hello world\n");
//   printk(KERN_ALERT"Traversal module is working..\n");
    printk(KERN_INFO "info Traversal module is working..\n");
    printk(KERN_DEBUG "debug Traversal module is working..\n");
#if 0
    current_head=&(current->tasks);
    list_for_each_entry(pos,current_head,tasks){
		count++;
		printk(KERN_ALERT"[process %d]: %s\'s pid is %d\n",count,pos->comm,pos->pid);
    }

    printk(KERN_ALERT"The number of process is:%d\n",count);
#endif
    return 0;
}

static void __exit traverse_exit(void)
{
      //printk(KERN_EMERG"traverse hello world exit\n");
      printk(KERN_INFO "info traverse hello world exit\n");
      printk(KERN_DEBUG "debug traverse hello world exit\n");
}

MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("traverse task list"); 
MODULE_AUTHOR("Zou Nan hai"); 

module_init(traverse_init); 
module_exit(traverse_exit);
