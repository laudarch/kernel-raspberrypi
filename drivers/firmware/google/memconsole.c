/*
 * memconsole.c
 *
 * Architecture-independent parts of the memory based BIOS console.
 *
 * Copyright 2014 Google Inc.
 */

#include <linux/init.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/module.h>

#include "memconsole.h"

static char *memconsole_baseaddr;
static size_t memconsole_length;

static ssize_t memconsole_read(struct file *filp, struct kobject *kobp,
			       struct bin_attribute *bin_attr, char *buf,
			       loff_t pos, size_t count)
{
	return memory_read_from_buffer(buf, count, &pos, memconsole_baseaddr,
				       memconsole_length);
}

static struct bin_attribute memconsole_bin_attr = {
	.attr = {.name = "log", .mode = 0444},
	.read = memconsole_read,
};

void memconsole_setup(void *baseaddr, size_t length)
{
	memconsole_baseaddr = baseaddr;
	memconsole_length = length;
}
EXPORT_SYMBOL(memconsole_setup);

int memconsole_sysfs_init(void)
{
	memconsole_bin_attr.size = memconsole_length;
	return sysfs_create_bin_file(firmware_kobj, &memconsole_bin_attr);
}
EXPORT_SYMBOL(memconsole_sysfs_init);

void memconsole_exit(void)
{
	sysfs_remove_bin_file(firmware_kobj, &memconsole_bin_attr);
}
EXPORT_SYMBOL(memconsole_exit);

MODULE_AUTHOR("Google, Inc.");
MODULE_LICENSE("GPL");
