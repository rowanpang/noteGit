#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/kthread.h>

static struct task_struct *thread;

static int threadfn(void *arg)
{
    int ret;
    int idx;
    idx = *(int*)arg;

    printk("In func %s,idx:%d,pid:%d\n",__func__,idx,current->pid);

    while(!kthread_should_stop()){
        ret++;
        printk("while In kthread func :%s,pid:%d,ret:%d,jiffies:%ld\n",__func__,current->pid,ret,jiffies);
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(3*HZ);
    }

    printk("exit func:%s,pid:%d\n",__func__,current->pid);
    return ret;
}


static int __init kmodstap_init(void)
{
    int ret = 0;
    int idx=5;

    printk("In func %s\n",__func__);
    thread  = kthread_create(threadfn, &idx , "kmodstap_kthread");
    if (!IS_ERR(thread)) {
        wake_up_process(thread);
    } else {
        thread = NULL;
        ret = -1;
        goto exit;
    }
    printk("thread create,ret:%d,pid:%d,tgid:%d,father:%d\n",ret,thread->pid,thread->tgid,thread->parent->pid);

exit:
    return ret;
}

static void __exit kmodstap_exit(void)
{
    int ret;
    printk("In func %s\n",__func__);

    if (thread) {
        ret = kthread_stop(thread);
    }

    printk("exit func %s, threadfun exit:%d\n",__func__,ret);
}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("kmodstap test");
MODULE_LICENSE("GPL");

module_init(kmodstap_init);
module_exit(kmodstap_exit);
