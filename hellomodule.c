#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
MODULE_LICENSE("GPL");

#define maj_no 400
#define min_no (1 << MINORBITS) - 1

#define noalloc

dev_t device;
unsigned int device_count = 2;
dev_t regdevice = MKDEV(maj_no, min_no);
static int hello_init(void)
{
	printk(KERN_ALERT "Hello, Kernel World!\n");
#ifdef alloc
	printk(KERN_ALERT "Using alloc_chrdev_region\n");
	if (alloc_chrdev_region(&device, min_no, device_count, "mydevice") < 0)
	{
		printk(KERN_ALERT "Failed to alloc device range\n");
		return -1;
	}
	printk(KERN_ALERT "Allocated M/m: %d/%d\n", MAJOR(device), MINOR(device));
#else
	printk(KERN_ALERT "Using register_chrdev_region\n");
	if (register_chrdev_region(regdevice, device_count, "mydevice") < 0)
	{
		printk(KERN_ALERT "Failed to reg device range\n");
		return -1;
	}
#endif
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Bye, Kernel World!\n");
#ifdef alloc
	printk(KERN_ALERT "Unalloc M/m: %d/%d\n", MAJOR(device), MINOR(device));
	unregister_chrdev_region(device, device_count);
#else	
	printk(KERN_ALERT "Unreg M/m: %d/%d\n", MAJOR(regdevice), MINOR(regdevice));
	unregister_chrdev_region(regdevice, device_count);
#endif
	return;
}

module_init(hello_init);
module_exit(hello_exit);
