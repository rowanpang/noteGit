#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/kthread.h>

static struct task_struct *thread;

static int threadfn(void *p)
{
	int *ret = p;
	printk("In func %s\n",__func__);
	*ret = 10;
	printk("threadfn pid:%d\n",current->pid);	

	while(!kthread_should_stop()){
		set_current_state(TASK_UNINTERRUPTIBLE);
		schedule_timeout(20*HZ);
				
		printk("threadfn pid:%d wakeup,and sleep immedialy\n",current->pid);	
	}

	return *ret;
}


static int __init hung_init(void)
{
	int ret = 0;
	printk("In func %s\n",__func__);
	thread  = kthread_create(threadfn, &ret , "hung_kthread"); 
	if (!IS_ERR(thread)){						  
		wake_up_process(thread);					   
	}else{
		thread = NULL;
	}
	printk("thread create,ret:%d,pid:%d,tgid:%d,father:%d\n",ret,thread->pid,thread->tgid,thread->parent->pid);

	return 0;
}

static void __exit hung_exit(void)
{ 
	printk("In func %s\n",__func__); 
	kthread_stop(thread);
}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("hung test");
MODULE_LICENSE("GPL");

module_init(hung_init);
module_exit(hung_exit);
