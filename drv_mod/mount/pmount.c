#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/kthread.h>

static struct task_struct *thread;
static DECLARE_COMPLETION(setup_done);

static int threadfn(void *p)
{
	int *ret = p;
	printk("In func %s\n",__func__);
	*ret = 10;
	complete(&setup_done);

	printk("pid:%d\n",current->pid);	

	while(!kthread_should_stop()){
		printk("while In kthread func %s\n",__func__);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(3*HZ);
	}

	printk("exit func %s\n",__func__);
	return *ret;
}


static int __init pmount_init(void)
{
	int ret = 0;
	printk("In func %s\n",__func__);
	thread  = kthread_create(threadfn, &ret , "pmount_kthread"); 
	if (!IS_ERR(thread)){						  
		wake_up_process(thread);					   
		wait_for_completion(&setup_done);
	}else{
		thread = NULL;
	}
	
	printk("after thread,ret:%d,pid:%d,tgid:%d,father:%d\n",ret,thread->pid,thread->tgid,thread->parent->pid);
	return 0;
}

static void __exit pmount_exit(void)
{
	printk("In func %s\n",__func__);
	kthread_stop(thread);

	printk("exit func %s\n",__func__);
}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("pmount test");
MODULE_LICENSE("GPL");

module_init(pmount_init);
module_exit(pmount_exit);
