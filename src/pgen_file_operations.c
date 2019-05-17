#include <linux/fs.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

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

static int is_used = 0;

static ssize_t pgen_read(struct file *file, char __user *buffer, size_t len, loff_t *offset)
{
	u8 	value;
	char*	buff;
	size_t 	u8_len = pgen_base64_len(sizeof(u8));

	buff = kmalloc(u8_len, GFP_KERNEL);
	if (!buff) {
		return (-ENOMEM);
	}

	get_random_bytes(&value, sizeof(u8));
	pgen_base64_encode(&value, sizeof(u8), buff);

	if (copy_to_user(buffer, buff, u8_len < len ? u8_len : len) != 0) {
		LOG(KERN_WARNING, "couldn't copy to user space");
		return (-EFAULT);
	}

	return (u8_len < len ? u8_len : len);
}

static int pgen_open(struct inode *node, struct file *file)
{
	if (is_used != 0)
		return (-EBUSY);

	is_used = 1;

	LOG(KERN_INFO, "open()\n");
	return (0);
}

static int pgen_release(struct inode *node, struct file *file)
{
	is_used = 0;

	LOG(KERN_INFO, "release()\n");
	return (0);
}
