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
//  FND driver region ( 7 segment ) 
//
/************************************************************************/
volatile unsigned short  *fnd_base;

int fnd_open(struct inode *inode, struct file *pfile) 
{

	printk("[fnd_open] - success.\n");
	if (request_mem_region(FND_BASE_ADDRESS ,ADDRESS_MAP_SIZE,"CNLED") == NULL)
		return -EBUSY;

	fnd_base = ioremap(FND_BASE_ADDRESS ,ADDRESS_MAP_SIZE);
	if ( fnd_base == NULL)
	{
		release_mem_region(FND_BASE_ADDRESS, ADDRESS_MAP_SIZE);
		return -ENOMEM;
	}
	
		
	return 0;
}

ssize_t fnd_read(struct file *pfile, char *buf, size_t count, loff_t *filePos)
{
	unsigned short wdata;
	unsigned int ret;
	wdata = readw(fnd_base);
	ret = copy_to_user(buf,&wdata,2);	
	return 2;
}
ssize_t fnd_write(struct file *pfile, const char *buf,size_t count, loff_t *filePos)
{

	unsigned short wdata;
	unsigned int ret;
	ret = copy_from_user(&wdata ,buf,2);	
	writew(wdata,fnd_base);
	return count;
}

int fnd_release(struct inode *inode , struct file *pfile)
{

	if (fnd_base != NULL )
	{
		iounmap(fnd_base);
		fnd_base = NULL;
		release_mem_region(FND_BASE_ADDRESS , ADDRESS_MAP_SIZE);

	}
	return 0;
}


struct file_operations fnd_fops = {
	.owner		= THIS_MODULE,
	.open		= fnd_open,
	.write		= fnd_write,
	.read		= fnd_read,
	.release	= fnd_release
};

struct miscdevice fnd_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_NAME_FND,
    .fops = &fnd_fops,
};

static int __init fnd_init(void)
{
    int error;

    error = misc_register(&fnd_device);
    if (error) {
        pr_err("can't misc_register :(\n");
        return error;
    }

    return 0;
}

static void __exit fnd_exit(void)
{
    misc_deregister(&fnd_device);
}

module_init(fnd_init)
module_exit(fnd_exit)

MODULE_DESCRIPTION("fnd Driver");
MODULE_AUTHOR("cndi");
MODULE_LICENSE("GPL");

