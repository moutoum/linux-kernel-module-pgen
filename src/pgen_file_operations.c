#include <linux/fs.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "pgen.h"

// prototype definitions used to fill the file operations structure.
static ssize_t pgen_read(struct file *file, char __user *buffer,
		         size_t len, loff_t *offset);
static int pgen_open(struct inode *node, struct file *file);
static int pgen_release(struct inode *, struct file *);

// pgen_fops describes the allowed operations available for a file.
static const struct file_operations pgen_fops = {
	.owner 	 = THIS_MODULE,
	.read	 = pgen_read,
	.open	 = pgen_open,
	.release = pgen_release,
};

// pgen_misc describes the misc device creation and behaviors.
// It is used to create the /dev/pgen file.
struct miscdevice pgen_misc = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= MNAME,
	.fops	= &pgen_fops,
	.mode	= 0644,
};

// is_used is a boolean that locks the access to
// the device if it is already in use.
static int 	is_used = 0;

// b64_buffer is used to store the encoded base64
// data.
static char* 	b64_buffer = NULL;
static ssize_t	b64_buffer_idx = -1;

// payload_size is a module parameter. It is used to choose the
// size of a payload in term of number of bytes randomly generated.
size_t 		payload_size = 1;

// payload_buffer is used to collect randomly generated data.
static char*	payload_buffer = NULL;

// pgen_read is the function used to read from the file operations
// structure.
// The function generates randomly some bytes and encodes them in
// base64 before sending them in the user space.
static ssize_t pgen_read(struct file*	file,
	  		 char __user*	buffer,
			 size_t 	len,
			 loff_t*	offset)
{
	size_t		encoded_data_len, formatted_data_len;
	unsigned long 	copy_ret;
	size_t 		bytes_wrote;

	encoded_data_len   = pgen_base64_len(payload_size);
	formatted_data_len = encoded_data_len + 2; // \n\0
	bytes_wrote = 0;

	do {
		if (b64_buffer_idx == -1) {
			get_random_bytes(payload_buffer, payload_size);
			pgen_base64_encode(payload_buffer, payload_size,
					   b64_buffer);
			b64_buffer[encoded_data_len    ] = '\n';
			b64_buffer[encoded_data_len + 1] = '\0';
			b64_buffer_idx = 0;
		}

		copy_ret = copy_to_user(
				buffer,
				b64_buffer + b64_buffer_idx,
				min(len - bytes_wrote,
				    formatted_data_len - b64_buffer_idx));
		if (copy_ret != 0) {
			LOG(KERN_WARNING, "couldn't copy to user space");
			return (-EFAULT);
		}

		bytes_wrote += min(len, formatted_data_len - b64_buffer_idx);
		b64_buffer_idx += min(len, formatted_data_len - b64_buffer_idx);
		if (b64_buffer_idx >= formatted_data_len) {
			b64_buffer_idx = -1;
		}
	} while (bytes_wrote < len);

	return (bytes_wrote);
}

static int pgen_open(struct inode *node,
		     struct file *file)
{
	if (is_used != 0)
		return (-EBUSY);

	b64_buffer = kmalloc(
			pgen_base64_len(payload_size) + 2, // \n\0
			GFP_KERNEL);
	if (!b64_buffer) {
		LOG(KERN_ERR, "couldn't allocate memory: ENOMEM\n");
		return (-ENOMEM);
	}

	payload_buffer = kmalloc(payload_size, GFP_KERNEL);
	if (!payload_buffer) {
		kfree(b64_buffer);
		LOG(KERN_ERR, "couldn't allocate memory: ENOMEM\n");
		return (-ENOMEM);
	}

	is_used = 1;
	return (0);
}

static int pgen_release(struct inode *node, 
			struct file *file)
{
	kfree(payload_buffer);
	kfree(b64_buffer);
	is_used = 0;
	return (0);
}
