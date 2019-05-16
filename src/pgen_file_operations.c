#include <linux/fs.h>

#include "pgen.h"

static ssize_t pgen_read(struct file *file, char __user *buffer, size_t len, loff_t *offset);
static int pgen_open(struct inode *node, struct file *file);
static int pgen_release(struct inode *, struct file *);

static const struct file_operations pgen_fops = {
	.owner 	 = THIS_MODULE,
	.read	 = pgen_read,
	.open	 = pgen_open,
	.release = pgen_release,
};

struct miscdevice pgen_misc = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= MNAME,
	.fops	= &pgen_fops,
	.mode	= 0644,
};

static ssize_t pgen_read(struct file *file, char __user *buffer, size_t len, loff_t *offset)
{
	LOG(KERN_INFO, "read()\n");
	return (0);
}

static int pgen_open(struct inode *node, struct file *file)
{
	LOG(KERN_INFO, "open()\n");
	return (0);
}

static int pgen_release(struct inode *node, struct file *file)
{
	LOG(KERN_INFO, "release()\n");
	return (0);
}
