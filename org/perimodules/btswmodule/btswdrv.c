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
//  button sw driver region ( 4 bit )
//
/************************************************************************/
volatile unsigned short  *btsw_base;

int btsw_open(struct inode *inode, struct file *pfile) 
{

	printk("[btsw_open] - success.\n");
	if (request_mem_region(BTN_SW_BASE_ADDRESS ,ADDRESS_MAP_SIZE,"CNMLED") == NULL)
		return -EBUSY;

	btsw_base = ioremap(BTN_SW_BASE_ADDRESS ,ADDRESS_MAP_SIZE);
	if ( btsw_base == NULL)
	{
		release_mem_region(BTN_SW_BASE_ADDRESS, ADDRESS_MAP_SIZE);
		return -ENOMEM;
	}
	
		
	return 0;
}

ssize_t btsw_read(struct file *pfile, char *buf, size_t count, loff_t *filePos)
{
	unsigned short wdata;
	unsigned int ret;
	wdata = readw(btsw_base);
	ret = copy_to_user(buf,&wdata,2);	
	wdata &= 0xFF; // 1byte is valid 
	return 2;
}



int btsw_release(struct inode *inode , struct file *pfile)
{

	if (btsw_base != NULL )
	{
		iounmap(btsw_base);
		btsw_base = NULL;
		release_mem_region(BTN_SW_BASE_ADDRESS , ADDRESS_MAP_SIZE);

	}
	return 0;
}


struct  file_operations btsw_fops = {
	.owner		= THIS_MODULE,
	.open		= btsw_open,
	.read		= btsw_read,
	.release	= btsw_release
};
 
 
 
struct miscdevice btsw_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_NAME_BTN_SW,
    .fops = &btsw_fops,
};
 
static int __init btsw_init(void)
{
    int error;
 
    error = misc_register(&btsw_device);
    if (error) {
        pr_err("can't misc_register :(\n");
        return error;
    }
 
    return 0;
}
 
static void __exit btsw_exit(void)
{
    misc_deregister(&btsw_device);
}
 
module_init(btsw_init)
module_exit(btsw_exit)
 
MODULE_DESCRIPTION("button switch Driver");
MODULE_AUTHOR("cndi");
MODULE_LICENSE("GPL");
