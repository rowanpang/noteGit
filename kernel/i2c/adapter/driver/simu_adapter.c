
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>

#define BUF_SIZE 128
char buf[BUF_SIZE]={0};
int valid_count = 0;
/* Return negative errno on error. */
static s32 simu_smbus_xfer(struct i2c_adapter *adap, u16 addr,
		       unsigned short flags, char read_write, u8 command,
		       int size, union i2c_smbus_data *data)
{
	printk(KERN_DEBUG "In func %s\n",__FUNCTION__);	

	return 0;
}

static s32 simu_master_xfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num) 
{
	int ret = 0;
	printk(KERN_DEBUG "In func %s\n",__FUNCTION__);	
	printk(KERN_DEBUG "cmd:%c,size:%d\n",(msgs->flags & I2C_M_RD)?'R':'W',msgs->len);	

	if(msgs->flags & I2C_M_RD){
		if(valid_count > 0){
			ret = (valid_count > msgs->len) ? msgs->len : valid_count ;
			memcpy(msgs->buf,buf,ret);
			valid_count -= ret;
		}else{
			ret = 0;
		}
	}else{
		ret = (BUF_SIZE > msgs->len) ? msgs->len : BUF_SIZE;
		memcpy(buf,msgs->buf,ret);
		valid_count = ret;	
		ret = (BUF_SIZE < msgs->len) ? msgs->len : BUF_SIZE;
	}

	printk(KERN_DEBUG "ret:%d,valid_count:%d\n",ret,valid_count);	
	return ret;
}

static u32 simu_func(struct i2c_adapter *adapter)
{
	printk(KERN_DEBUG "In func %s\n",__FUNCTION__);	

	return I2C_FUNC_SMBUS_QUICK | I2C_FUNC_SMBUS_BYTE |
	       I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_WORD_DATA |
	       I2C_FUNC_SMBUS_BLOCK_DATA | I2C_FUNC_SMBUS_WRITE_I2C_BLOCK |
	       I2C_FUNC_SMBUS_PEC | I2C_FUNC_SMBUS_READ_I2C_BLOCK;
}

static const struct i2c_algorithm simu_algorithm = {
	.master_xfer = simu_master_xfer,
	.smbus_xfer	= simu_smbus_xfer,
	.functionality	= simu_func,
};

static struct i2c_adapter simu_adapter = {
	.owner = THIS_MODULE,
	.class = I2C_CLASS_HWMON | I2C_CLASS_SPD,
	.algo = &simu_algorithm,
	.name = "pangwz simu_adapter"
};

static int __init i2c_simu_init(void)
{
	int err = 0;
	printk(KERN_DEBUG "In func %s\n",__FUNCTION__);	
	err = i2c_add_adapter(&simu_adapter);
	if (err) {
		printk(KERN_ERR, "Failed to add SMBus adapter\n");
		return err;
	}
	return 0;
	
}

static void __exit i2c_simu_exit(void)
{
	printk(KERN_DEBUG "In func %s\n",__FUNCTION__);	
	i2c_del_adapter(&simu_adapter);
}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("adapter simulater");
MODULE_LICENSE("GPL");

module_init(i2c_simu_init);
module_exit(i2c_simu_exit);
