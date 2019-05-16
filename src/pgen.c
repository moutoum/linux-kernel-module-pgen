#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include "pgen.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Maxence Moutoussamy <maxence.moutoussamy1@gmail.com>");
MODULE_DESCRIPTION("This module is used to generate u8 values encoded in base64.");
MODULE_VERSION("1.0");

static __init int pgen_init(void)
{
	int error_code;

	error_code = misc_register(&pgen_misc);
	if (error_code < 0) {
		LOG(KERN_ERR, "couldn't register misc device: %d\n", error_code);
		return (error_code);
	}

	LOG(KERN_INFO, "initialized\n");
	return (0);
}

static __exit void pgen_exit(void)
{
	misc_deregister(&pgen_misc);
	LOG(KERN_INFO, "exited\n");
}

module_init(pgen_init);
module_exit(pgen_exit);
