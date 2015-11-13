#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include "peripheral.h"

/************************************************************************/
// 
//  Matrix LED driver region ( column select 0~ 4 , row data 0~6)
//
/************************************************************************/
volatile unsigned short  *mled_base;

int mled_open(struct inode *inode, struct file *pfile) 
{

	printk("[mled_open] - success.\n");
	if (request_mem_region(MATRIX_LED_BASE_ADDRESS ,ADDRESS_MAP_SIZE,"CNMLED") == NULL)
		return -EBUSY;

	mled_base = ioremap(MATRIX_LED_BASE_ADDRESS ,ADDRESS_MAP_SIZE);
	if ( mled_base == NULL)
	{
		release_mem_region(MATRIX_LED_BASE_ADDRESS, ADDRESS_MAP_SIZE);
		return -ENOMEM;
	}
	
		
	return 0;
}

ssize_t mled_read(struct file *pfile, char *buf, size_t count, loff_t *filePos)
{
	unsigned short wdata;
	unsigned int ret;
	wdata = readw(mled_base);
	ret = copy_to_user(buf,&wdata,2);	
	return 2;
}

ssize_t mled_write(struct file *pfile, const char *buf,size_t count, loff_t *filePos)
{

	unsigned short wdata[2];
	unsigned int ret;
	ret = copy_from_user((unsigned char*)wdata ,buf,4);	
	writew(wdata[0],mled_base);
	writew(wdata[1],mled_base +1);
	return count;
}


int mled_release(struct inode *inode , struct file *pfile)
{

	if (mled_base != NULL )
	{
		iounmap(mled_base);
		mled_base = NULL;
		release_mem_region(MATRIX_LED_BASE_ADDRESS , ADDRESS_MAP_SIZE);

	}
	return 0;
}


struct file_operations mled_fops = {
	.owner		= THIS_MODULE,
	.open		= mled_open,
	.write		= mled_write,
	.read		= mled_read,
	.release	= mled_release
};

struct miscdevice mled_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_NAME_MATRIX_LED,
    .fops = &mled_fops,
};

static int __init mled_init(void)
{
    int error;

    error = misc_register(&mled_device);
    if (error) {
        pr_err("can't misc_register :(\n");
        return error;
    }

    return 0;
}

static void __exit mled_exit(void)
{
    misc_deregister(&mled_device);
}

module_init(mled_init)
module_exit(mled_exit)

MODULE_DESCRIPTION("matrix led Driver");
MODULE_AUTHOR("cndi");
MODULE_LICENSE("GPL");

