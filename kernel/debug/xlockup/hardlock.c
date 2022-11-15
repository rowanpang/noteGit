#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/kthread.h>
#include <linux/hardirq.h>
#include <asm/paca.h>
#include <asm/reg.h>


static int delays = 25;
module_param(delays,int,0600);

static int __init hardlock_init(void)
{
    int ret = 0;
    unsigned long dd = delays*HZ/5;
    unsigned long dtmp = jiffies;
    unsigned long dtotal = jiffies+delays*HZ;
    int cpu;
    unsigned long msr;

    msr = mfmsr();
    printk("In func %s pre lid, cpu:%d, dtotal:%ld, delay:%ds, msr.ee:%d \n",__func__,get_cpu(),dtotal,delays,(msr&MSR_EE?1:0));
    local_irq_disable();

    msr = mfmsr();
    printk("In func %s after lid, cpu:%d, dtotal:%ld, delay:%ds, msr.ee:%d \n",__func__,get_cpu(),dtotal,delays,(msr&MSR_EE?1:0));
    while (time_before(jiffies,dtotal)) {
    	cpu = get_cpu();
	msr = mfmsr();
   	printk("cur cpu: %d, jiffies:%ld, snmi: %d, softmask:%d, msr.ee:%d, irqhed:%#x\n",\
					cpu,\
					jiffies,\
					per_cpu(irq_stat.soft_nmi_irqs,cpu),\
					get_paca()->irq_soft_mask,\
					(msr&MSR_EE?1:0)\
					,get_paca()->irq_happened\
					);
	put_cpu();
	dtmp += dd;
	while (time_before(jiffies,dtmp));
    }

    local_irq_enable();
    cpu = get_cpu();
    printk("After lie cur cpu: %d, jiffies:%ld, snmi: %d, softmask:%d msr.ee:%d\n"\
		    	,cpu\
			,jiffies\
			,per_cpu(irq_stat.soft_nmi_irqs,cpu)\
			,get_paca()->irq_soft_mask\
			,(msr&MSR_EE?1:0)\
			); 
    return ret;
}

static void __exit hardlock_exit(void)
{ 
    printk("In func %s\n",__func__); 
}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("hardlock test");
MODULE_LICENSE("GPL");

module_init(hardlock_init);
module_exit(hardlock_exit);
