#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sem.h>
#include <linux/list.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define STR_LEN  128

static char str[128]={"pangwz12345678"};


static void *my_seq_start(struct seq_file *s, loff_t *pos)
{
    printk("In %s\n",__FUNCTION__);
    if(*pos < strlen(str)){
        return pos;
    }

    return NULL;
}

static void *my_seq_next(struct seq_file *f, void *v, loff_t *pos)
{
    printk("In %s\n",__FUNCTION__);
    (*pos)++;
    if(*pos >= strlen(str)){
        return NULL;
    }
    return pos;
}

static int my_seq_show(struct seq_file *f, void *v)
{
    loff_t pos = *(loff_t*)v;
    printk("In %s\n",__FUNCTION__);

    seq_printf(f,"%c\n",*(str+pos));

    return 0;
}

static void my_seq_stop(struct seq_file *f, void *v)
{
    printk("In %s\n",__FUNCTION__);
}

static struct seq_operations my_seq_ops = {
    .start = my_seq_start,
    .next = my_seq_next,
    .stop = my_seq_stop,
    .show = my_seq_show
};

static int seq_file_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &my_seq_ops);
};

static int single_seq_show(struct seq_file *f, void *v)
{
    printk("In %s\n",__FUNCTION__);

    seq_printf(f,"%s\n",str);

    return 0;
}

static int single_file_open(struct inode *inode, struct file *file)
{
    return single_open(file, &single_seq_show,NULL);
};

static struct file_operations seq_file_ops = {
    .owner = THIS_MODULE,
    .open = seq_file_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release
};

static struct file_operations single_file_ops = {
    .owner = THIS_MODULE,
    .open = single_file_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release
};

int qxl_release_ctl=0;

static int procfs_qxl_release_open(struct inode *inode,struct file *file)
{
        file->private_data = &qxl_release_ctl;
        return 0;
}

static ssize_t procfs_qxl_release_read(struct file *file,char __user *buf,size_t size,loff_t *ppos)
{
        char ibuf[10];
        int ret;
        int err;
        int *ptr = (int*) file->private_data;
        if (*ppos >= 1 ) {
            return 0;
        }
        ret = snprintf(ibuf,sizeof(ibuf),"%d\n",*ptr);
        err = copy_to_user(buf,ibuf,ret);

        *ppos += ret;
        return ret;
}

static ssize_t procfs_qxl_release_write(struct file *file, const char __user *buf,size_t size,loff_t *ppos)
{
        char ibuf[10]={0};
        int ret;
        int copy;
        int *ptr = (int*) file->private_data;

        if (*ppos >= 1 ) {
            return 0;
        }

        copy = size < 10 ? size : 10 ;
        ret = copy_from_user(ibuf,buf,copy);

        ret = kstrtoint(ibuf,10,ptr);

        *ppos += copy;

        printk("copy:%d,qxl_release_ctl:%d\n",copy,qxl_release_ctl);
        return copy ;
}

static struct file_operations qxl_release_fop = {
        .owner = THIS_MODULE,
        .open = procfs_qxl_release_open,
        .write = procfs_qxl_release_write,
        .read = procfs_qxl_release_read,
};

static int __init  seq_file_init(void)
{
    proc_create("seq_test_rowan", 0, NULL, &seq_file_ops);
    proc_create("single_test_rowan", 0, NULL, &single_file_ops);
    proc_create("qxl_releasectl",0,NULL,&qxl_release_fop);

    return 0;
}

static void __exit seq_file_exit(void)
{
    remove_proc_entry("seq_test_rowan",NULL);
    remove_proc_entry("single_test_rowan",NULL);
    remove_proc_entry("qxl_releasectl",NULL);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("seq file test");
MODULE_AUTHOR("rowan pang");

module_init(seq_file_init);
module_exit(seq_file_exit);
