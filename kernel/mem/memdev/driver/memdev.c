/*********************************************************************************************
#####         �Ϻ�Ƕ��ʽ��԰-�������̳�         #####
#####                             www.embedclub.com                              #####
#####                      http://embedclub.taobao.com                      #####

* File��		memdev.c
* Author:		Hanson
* Desc��	virual memory  device driver 
* History:	May 20th 2011
*********************************************************************************************/
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/device.h>

#include "memdev.h"

static mem_major = MEMDEV_MAJOR;

module_param(mem_major, int, S_IRUGO);

struct mem_dev *mem_devp; /*�豸�ṹ��ָ��*/

struct cdev cdev[MEMDEV_NR_DEVS]; 
/*�ļ��򿪺���*/
int mem_open(struct inode *inode, struct file *filp)
{
    struct mem_dev *dev;
    
    /*��ȡ���豸��*/
    int num = MINOR(inode->i_rdev);

    if (num >= MEMDEV_NR_DEVS) 
            return -ENODEV;
    dev = &mem_devp[num];
    
    /*���豸�����ṹָ�븳ֵ���ļ�˽������ָ��*/
    filp->private_data = dev;
    
    return 0; 
}

/*�ļ��ͷź���*/
int mem_release(struct inode *inode, struct file *filp)
{
  return 0;
}

/*������*/
static ssize_t mem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;
  struct mem_dev *dev = filp->private_data; /*����豸�ṹ��ָ��*/

  /*�ж϶�λ���Ƿ���Ч*/
  if (p >= MEMDEV_SIZE)
    return 0;
  if (count > MEMDEV_SIZE - p)
    count = MEMDEV_SIZE - p;

  /*�����ݵ��û��ռ�*/
  if (copy_to_user(buf, (void*)(dev->data + p), count))
  {
    ret =  - EFAULT;
  }
  else
  {
    *ppos += count;
    ret = count;
    
    printk(KERN_INFO "read %d bytes(s) from postion  %d\n", count, p);
  }

  return ret;
}

/*д����*/
static ssize_t mem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;
  struct mem_dev *dev = filp->private_data; /*����豸�ṹ��ָ��*/
  
  /*�����ͻ�ȡ��Ч��д����*/
  if (p >= MEMDEV_SIZE)
    return 0;
  if (count > MEMDEV_SIZE - p)
    count = MEMDEV_SIZE - p;
    
  /*���û��ռ�д������*/
  if (copy_from_user(dev->data + p, buf, count))
    ret =  - EFAULT;
  else
  {
    *ppos += count;
    ret = count;
    
    printk(KERN_INFO "written %d bytes(s) to postion %d\n", count, p);
  }

  return ret;
}

/* seek�ļ���λ���� */
static loff_t mem_llseek(struct file *filp, loff_t offset, int whence)
{ 
    loff_t newpos;

    switch(whence) {
      case 0: /* SEEK_SET */
        newpos = offset;
        break;

      case 1: /* SEEK_CUR */
        newpos = filp->f_pos + offset;
        break;

      case 2: /* SEEK_END */
        newpos = MEMDEV_SIZE -1 + offset;
        break;

      default: /* can't happen */
        return -EINVAL;
    }
    if ((newpos<0) || (newpos>MEMDEV_SIZE))
    	return -EINVAL;
    	
    filp->f_pos = newpos;
    return newpos;

}

/*�ļ������ṹ��*/
static const struct file_operations mem_fops =
{
  .owner = THIS_MODULE,
  .llseek = mem_llseek,
  .read = mem_read,
  .write = mem_write,
  .open = mem_open,
  .release = mem_release,
};
struct class *mem_class;

static int memdev_init(void)
{
  int result;
  int i;

  dev_t devno = MKDEV(mem_major, 0);

  if (mem_major){
	result = register_chrdev_region(devno, MEMDEV_NR_DEVS, "memdev");
  }
  else{
    result = alloc_chrdev_region(&devno, 0, MEMDEV_NR_DEVS, "memdev");
    mem_major = MAJOR(devno);
  }  
  
  if (result < 0)
    return result;

  mem_devp = kmalloc(MEMDEV_NR_DEVS * sizeof(struct mem_dev), GFP_KERNEL);
  if (!mem_devp){
    result =  - ENOMEM;
    goto fail_malloc;
  }

  memset(mem_devp, 0, sizeof(struct mem_dev));
 
  mem_class = class_create(THIS_MODULE, "mem_dev"); 
  for (i=0; i < MEMDEV_NR_DEVS; i++){
		dev_t devt_mem = MKDEV(mem_major, i);
 		cdev_init(&cdev[i], &mem_fops);
  		cdev[i].owner = THIS_MODULE;
 		cdev_add(&cdev[i],devt_mem, MEMDEV_NR_DEVS);

		struct device *dev_mem = NULL; 
		dev_mem=device_create(mem_class,NULL,devt_mem,&mem_devp[i],"memdev_%d",i);
		//dev_mem=device_create(NULL,NULL,devt_mem,&mem_devp[i],"memdev_%d",i);

        mem_devp[i].size = MEMDEV_SIZE;
        mem_devp[i].data = kmalloc(MEMDEV_SIZE, GFP_KERNEL);
		mem_devp[i].dev = dev_mem;
		mem_devp[i].devt = devt_mem;
        memset(mem_devp[i].data, 0, MEMDEV_SIZE);
  }
  printk("memdev driver installed, with major %d\n", mem_major);
  printk("the device name is %s\n","memdev");
  return 0;

  fail_malloc: 
	  unregister_chrdev_region(devno, 1);
  
  return result;
}

/*ģ��ж�غ���*/
static void memdev_exit(void)
{
  int i;
  for (i=0; i < MEMDEV_NR_DEVS; i++) 
  {
  	cdev_del(&cdev[i]);   /*ע���豸*/
	kfree(mem_devp[i].data);
    device_destroy(mem_class,mem_devp[i].devt);
  }
  class_destroy(mem_class);

  kfree(mem_devp);     /*�ͷ��豸�ṹ���ڴ�*/
  unregister_chrdev_region(MKDEV(mem_major, 0), MEMDEV_NR_DEVS); /*�ͷ��豸��*/
  printk("memdev driver uninstalled\n");
}

MODULE_AUTHOR("Hanson He");
MODULE_LICENSE("GPL");

module_init(memdev_init);
module_exit(memdev_exit);