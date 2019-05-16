#ifndef __PGEN_H__
#define __PGEN_H__

#include <linux/miscdevice.h>

#define MNAME "pgen"

#define LOG(level, message, ...) printk(level MNAME ": " message, ##__VA_ARGS__)

extern struct miscdevice pgen_misc;

#endif // __PGEN_H__
