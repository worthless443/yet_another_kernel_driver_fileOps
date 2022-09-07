#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>


//
//int register_chrdev(const char *name); 
//
//
//int register_chrdev(const char *name) {
//	dev_t dev = 0;
//	alloc_chrdev_region(&dev, 0, 1, name);
//	return  MAJOR(dev)
//}
//
dev_t dev = 0;
struct cdev c_dev;
struct class dev_class;

static int      __init driver1_init_(void);
static void     __exit driver1_exit_(void);
static ssize_t  read_f(struct file *filp, char __user *buf, size_t len,loff_t * off);
static int      open_f(struct inode *inode, struct file *file);
static int      release_f(struct inode *inode, struct file *file);
static ssize_t      write_f(struct file *filp, const char *buf, size_t len, loff_t * off);

struct file_operations fops;

// TODO define ioctl to interface with
void set_fops(struct file_operations *fops) {
	fops->read = read_f;
	fops->open = open_f;
	fops->release = release_f;
	fops->write = write_f;
}

int val = 0;

static ssize_t  write_f(struct file *filp, const char *buf, size_t len, loff_t * off) {
	printk(KERN_INFO "writing; len = %d", (int)len);
	///fwrite(buf,1,len,filp);
	return len;
}

static ssize_t  read_f(struct file *filp, char *buf, size_t len,loff_t * off) {
    int errors = 0;
    int m_len = 7;
    val +=1;
    errors = copy_to_user(buf, "nigger ", m_len);
    printk(KERN_INFO "read %d",  val);
    if(val == m_len - 1) {
	    errors = copy_to_user(buf, "\n", 2);
	    return 2;
    }
    return val < m_len ? m_len : 0;
    //return errors == 0 ? message_len : -EFAULT;

}
static int  open_f(struct inode *inode, struct file *file) {
	printk(KERN_INFO "fuck it open!!!!");
	return 0;
}
static int  release_f(struct inode *inode, struct file *file) {
	val = 0;
	printk(KERN_INFO "fuck it release!!!!");
	return 0;
}

static int __init driver1_init_(void) {
	int ret = 0;
	if(alloc_chrdev_region(&dev, 0, 1, "driver2")<0) {
		printk(KERN_NOTICE "can not alloc\n"); 
		return -1;
	}
	set_fops(&fops);
	cdev_init(&c_dev, &fops);
	ret = cdev_add(&c_dev, dev, 1);
	printk(KERN_INFO "init err %d\n", ret);
	printk(KERN_INFO "driver1 Major = %d && Minor = %d\n", MAJOR(dev), MINOR(dev));
	return 0;
}
void __exit driver1_exit_(void) {
	cdev_del(&c_dev);
	printk(KERN_NOTICE "called del()\n");
	unregister_chrdev_region(dev, 1);
	printk(KERN_NOTICE "called unreg()\n");
}

module_init(driver1_init_);
module_exit(driver1_exit_);

MODULE_LICENSE("GPL3");
MODULE_VERSION("0.1");
