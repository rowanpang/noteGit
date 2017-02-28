#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>

static struct task_struct *threadA;
static DECLARE_COMPLETION(setup_done);
static DEFINE_SPINLOCK(my_lock);

int i = 0; 

static int threadfnA(void *p)
{
    int *ret = p;
    printk("In func %s\n",__func__);
    *ret = 10;

    while(!kthread_should_stop()){
        spin_lock(&my_lock);
        printk("funcA spinlock\n");
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(1*HZ);
        spin_unlock(&my_lock);
        printk("funcA spinulock\n");
    }

    printk("exit func %s\n",__func__);
    return *ret;
}


static int __init pmount_init(void)
{
    int ret = 0;
    printk("In func %s\n",__func__);
    threadA  = kthread_create(threadfnA, &ret , "pmount_kthreadA"); 
    if (!IS_ERR(threadA)){                          
        wake_up_process(threadA);                       
    }else{
        threadA = NULL;
    }

    printk("after threadA,ret:%d,pid:%d,tgid:%d,father:%d\n",ret,threadA->pid,threadA->tgid,threadA->parent->pid);
    return 0;
}

static void __exit pmount_exit(void)
{
    printk("In func %s\n",__func__);
    kthread_stop(threadA);

    printk("exit func %s\n",__func__);
}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("pmount test");
MODULE_LICENSE("GPL");

module_init(pmount_init);
module_exit(pmount_exit);
