#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include "pgen.h"

// module descriptions.
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Maxence Moutoussamy <maxence.moutoussamy1@gmail.com>");
MODULE_DESCRIPTION("This module is used to generate u8 " \
		   "values encoded in base64.");
MODULE_VERSION("1.0");

// module parameters.
module_param(payload_size, ulong, 0644);
MODULE_PARM_DESC(payload_size, " this is the size of the payload generated");

// pgen_init initiliazes the module and creates the misc device.
static __init int pgen_init(void)
{
	int err;

	err = misc_register(&pgen_misc);
	if (err < 0) {
		LOG(KERN_ERR, "couldn't register misc device: %d\n", err);
		return (err);
	}

	LOG(KERN_INFO, "initialized\n");
	return (0);
}

// pgen_exit deregisters the misc device and exits the module.
static __exit void pgen_exit(void)
{
	misc_deregister(&pgen_misc);
	LOG(KERN_INFO, "exited\n");
}

module_init(pgen_init);
module_exit(pgen_exit);
