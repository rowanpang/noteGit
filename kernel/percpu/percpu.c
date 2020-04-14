#include <linux/module.h>
#include <linux/init.h>
#include <linux/percpu.h>
#include <linux/cpumask.h>

static DEFINE_PER_CPU(long, cpuvar) = 10;
static long __percpu *cpualloc;

static int percpu_test_init(void)
{
    int cpu;
    pr_info("module loaded at 0x%p\n", percpu_test_init);
    /* modify the cpuvar value */
    for_each_possible_cpu(cpu)
	per_cpu(cpuvar, cpu) = cpu;
    pr_info("cpuvar: %p\n",&cpuvar);

    pr_info("init: cpuvar on cpu%d  = %ld\n", smp_processor_id(), get_cpu_var(cpuvar)++);
    put_cpu_var(cpuvar);

    cpualloc = alloc_percpu(long);
    pr_info("cpualloc: %p,valued: 100\n",cpualloc);
    for_each_possible_cpu(cpu)
	*per_cpu_ptr(cpualloc, cpu) = 100;


    return 0;
}

static void percpu_test_exit(void)
{
    	int cpu;
	pr_info("exit module...\n");

	for_each_possible_cpu(cpu)
	    pr_info("cpuvar cpu%d = %ld\n", cpu, per_cpu(cpuvar, cpu));

	pr_info("exit: cpualloc = %ld\n", *per_cpu_ptr(cpualloc, smp_processor_id()));
	free_percpu(cpualloc);

	pr_info("Bye: module unloaded from 0x%p\n", percpu_test_exit);
}

module_init(percpu_test_init);
module_exit(percpu_test_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("pangwz");
MODULE_DESCRIPTION("for percpu test");

