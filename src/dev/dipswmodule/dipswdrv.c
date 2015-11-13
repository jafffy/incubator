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
//  Dip SW driver region ( 2 byte ) 
//
/************************************************************************/
volatile unsigned short  *dipsw_base;

int dipsw_open(struct inode *inode, struct file *pfile) 
{

	printk("[dipsw_open] - success.\n");
	if (request_mem_region(DIP_SW_BASE_ADDRESS ,ADDRESS_MAP_SIZE,"CNMLED") == NULL)
		return -EBUSY;

	dipsw_base = ioremap(DIP_SW_BASE_ADDRESS ,ADDRESS_MAP_SIZE);
	if ( dipsw_base == NULL)
	{
		release_mem_region(DIP_SW_BASE_ADDRESS, ADDRESS_MAP_SIZE);
		return -ENOMEM;
	}
	
		
	return 0;
}

ssize_t dipsw_read(struct file *pfile, char *buf, size_t count, loff_t *filePos)
{
	unsigned short rdata;
	unsigned int ret;
	rdata = readw(dipsw_base);
	ret = copy_to_user(buf,&rdata,2);	
	return 2;
}


int dipsw_release(struct inode *inode , struct file *pfile)
{

	if (dipsw_base != NULL )
	{
		iounmap(dipsw_base);
		dipsw_base = NULL;
		release_mem_region(DIP_SW_BASE_ADDRESS , ADDRESS_MAP_SIZE);

	}
	return 0;
}


struct file_operations dipsw_fops = {
	.owner		= THIS_MODULE,
	.open		= dipsw_open,
	.read		= dipsw_read,
	.release	= dipsw_release
};


struct miscdevice dipsw_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_NAME_DIP_SW,
    .fops = &dipsw_fops,
};

static int __init dipsw_init(void)
{
    int error;

    error = misc_register(&dipsw_device);
    if (error) {
        pr_err("can't misc_register :(\n");
        return error;
    }

    return 0;
}

static void __exit dipsw_exit(void)
{
    misc_deregister(&dipsw_device);
}

module_init(dipsw_init)
module_exit(dipsw_exit)

MODULE_DESCRIPTION("dip switch Driver");
MODULE_AUTHOR("cndi");
MODULE_LICENSE("GPL");

