// Created by Zhigang An on Apr. 15, 2017.
// Case Western Reserve university
// 
// Compile: make
// Insert module: sudo insmod HelloWorld_km.ko
// To change the default name: sudo insmod HelloWorld_km.ko name="NewName"
// See the info printed: sudo tail -f /var/log/kern.log
// Remove module: sudo rmmod HelloWorld_km.ko
//
// Print "Hello World!" message from kernel module.

// Function markup macros (__init, __exit).
#include <linux/init.h>
// Header for loading kernel modules into the kernel.
#include <linux/module.h>
// Types, macros, functions for the kernel.
#include <linux/kernel.h>

MODULE_LICENSE("BSD");
MODULE_AUTHOR("Zhigang An");
MODULE_DESCRIPTION("Hello World kernel module.");
MODULE_VERSION("1.0");

// Default name.
static char *name = "HelloWorld";
// S_IRUGO read only.
module_param(name, charp, S_IRUGO);
// Parameter description.
MODULE_PARM_DESC(name, "Name appears in /var/log/kern.log");

// Initialization function.
static int __init initialization(void)
{
	printk(KERN_INFO "%s: Hello World!\n", name);
	return 0;
}

// Cleanup function.
static void __exit finalization(void){
	printk(KERN_INFO "%s: Exit.\n", name);
}

// Kernel module must use the module_init() module_exit() macros.
module_init(initialization);
module_exit(finalization);
