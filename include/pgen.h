#ifndef __PGEN_H__
#define __PGEN_H__

#include <linux/miscdevice.h>

#define MNAME "pgen"

#define LOG(level, message, ...) printk(level MNAME ": " message, ##__VA_ARGS__)

extern struct miscdevice pgen_misc;
extern size_t payload_size;

size_t pgen_base64_len(size_t data_size);
void pgen_base64_encode(const unsigned char *cdata, size_t data_size, char *buffer);

#endif // __PGEN_H__
