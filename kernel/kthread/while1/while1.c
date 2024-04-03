#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct task_struct *thread;
static DECLARE_COMPLETION(setup_done);


static int threadfn(void *p)
{
	int *ret = p;
	int i = 0;
	printk("In func %s\n",__func__);
	*ret = 10;
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(2*HZ);
	complete(&setup_done);

	printk("threadfn pid:%d\n",current->pid);	

	while(!kthread_should_stop()){
		printk("delay 500ms: %5d\n",i);
		mdelay(500);
		i++;
	}

	printk("threadfn exit, pid:%d\n",current->pid);
	return *ret;
}


static int __init pmount_init(void)
{
	int ret = 0;
	printk("In func %s\n",__func__);
	thread  = kthread_create(threadfn, &ret , "pmount_kthread"); 
	if (!IS_ERR(thread)){						  
		wake_up_process(thread);					   
		//wait_for_completion(&setup_done);
	} else {
		thread = NULL;
	}
	printk("thread create,ret:%d,pid:%d,tgid:%d,father:%d\n",ret,thread->pid,thread->tgid,thread->parent->pid);

	return 0;
}

static void __exit pmount_exit(void)
{ 
	printk("In func %s\n",__func__); 
	schedule_timeout_interruptible(15*HZ);
	if (thread)
		kthread_stop(thread);
	printk("exit func %s\n",__func__);
}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("pmount test");
MODULE_LICENSE("GPL");

module_init(pmount_init);
module_exit(pmount_exit);
