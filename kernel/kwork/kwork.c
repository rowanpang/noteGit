#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/list.h>

struct workqueue_struct *workqueue_test;

static LIST_HEAD(myworks);
struct mywork {
    int id;
    struct list_head entry;
    struct work_struct work_test;
};

struct mywork* mywork_alloc_and_init(int id)
{
    struct mywork *wk = kzalloc(sizeof(struct mywork),GFP_KERNEL);
    wk->id = id;
    INIT_LIST_HEAD(&wk->entry);
    list_add(&wk->entry,&myworks);

    return wk;
}

void work_test_func(struct work_struct *work)
{
    struct mywork *wk = container_of(work,struct mywork,work_test);
    printk("%s(),mywork id: %d\n", __func__,wk->id);

    //mdelay(1000);
    //queue_work(workqueue_test, &work_test);
}



static int test_init(void)
{
    int i = 0;
    struct mywork *wk1 = mywork_alloc_and_init(i++);
    struct mywork *wk2 = mywork_alloc_and_init(i++);
    struct mywork *wk3 = mywork_alloc_and_init(i++);

    printk("Hello,world!\n");

    /* 1. 自己创建一个workqueue， 中间参数为0，默认配置 */
    workqueue_test = alloc_workqueue("workqueue_test", 0, 0);
    /* 2. 初始化一个工作项，并添加自己实现的函数 */
    INIT_WORK(&wk1->work_test, work_test_func);
    /* 3. 将自己的工作项添加到指定的工作队列去， 同时唤醒相应线程处理 */
    queue_work(workqueue_test, &wk1->work_test);

    INIT_WORK(&wk2->work_test, work_test_func);
    queue_work(system_wq, &wk2->work_test);

    INIT_WORK(&wk3->work_test, work_test_func);
    schedule_work(&wk3->work_test);
    schedule_work(&wk3->work_test);		//仅仅执行一次.

    return 0;
}

static void test_exit(void)
{
    struct mywork *wk;

    printk("\n");
    printk("Goodbye,cruel world!\n");
    destroy_workqueue(workqueue_test);

    list_for_each_entry(wk,&myworks,entry){
	printk("kfree mywork, id: %d\n",wk->id);
	kfree(wk);
    }

    printk("\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_AUTHOR("Vedic <FZKmxcz@163.com>");
MODULE_LICENSE("Dual BSD/GPL");
