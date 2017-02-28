#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/kthread.h>

static struct task_struct *thread;
static struct task_struct *threadA;
static DECLARE_COMPLETION(setup_done);

#define GOOD_FUNC 1

static int threadfn(void *p)
{
	int *ret = p;
	printk("In func %s\n",__func__);
	*ret = 10;
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(3*HZ);
	complete(&setup_done);

	printk("threadfn pid:%d\n",current->pid);	

#if GOOD_FUNC
	while(!kthread_should_stop()){
		printk("while In kthread func :%s,pid:%d\n",__func__,current->pid);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(3*HZ);
	}
#else
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(20*HZ);
#endif
	printk("exit func:%s,pid:%d\n",__func__,current->pid);
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
	}else{
		thread = NULL;
	}
	printk("thread create,ret:%d,pid:%d,tgid:%d,father:%d\n",ret,thread->pid,thread->tgid,thread->parent->pid);

	threadA  = kthread_create(threadfn, &ret , "pmount_kthreadA"); 
	if (!IS_ERR(threadA)){						  
		wake_up_process(threadA);					   
	}else{
		threadA = NULL;
	}

	printk("threadA create,ret:%d,pid:%d,tgid:%d,father:%d\n",ret,threadA->pid,threadA->tgid,threadA->parent->pid);

	return 0;
}

static void __exit pmount_exit(void)
{ 
	printk("In func %s\n",__func__); 
#if GOOD_FUNC 
	kthread_stop(thread);
	kthread_stop(threadA);
#endif

	printk("exit func %s\n",__func__);
}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("pmount test");
MODULE_LICENSE("GPL");

module_init(pmount_init);
module_exit(pmount_exit);
