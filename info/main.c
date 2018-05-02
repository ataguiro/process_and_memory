#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adam Taguirov <ataguiro@student.42.fr>");
MODULE_DESCRIPTION("Hello World module");

static int __init hello_init(void) {
	printk(KERN_INFO "Hello World !\n");
	struct task_struct *cur;

	for_each_process(cur) {
		printk("pid   : %ld\n", cur->pid);
		printk("state : %ld\n", cur->state);
		printk("stack : %p\n", cur->stack);
		printk("stack : %zu\n", cur->start_time);
		printk("child : none\n");
		printk("ppid  : %ld\n", cur->parent->pid);
		printk("ppid  : %ld\n", cur->parent->pid);
	}

	return 0;
}

static void __exit hello_cleanup(void) {
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
