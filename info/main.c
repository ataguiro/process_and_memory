#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/fs_struct.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adam Taguirov <ataguiro@student.42.fr>");
MODULE_DESCRIPTION("Hello World module");

static int __init hello_init(void) {
	printk(KERN_INFO "Hello World !\n");
	struct task_struct *cur;
	char buffer[PATH_MAX];

	for_each_process(cur) {
		printk("name  : %s\n", cur->comm);
		printk("pid   : %d\n", cur->pid);
		printk("state : %ld\n", cur->state);
		printk("stack : %p\n", cur->stack);
		printk("age   : %llu\n", cur->start_time);
		printk("child : none\n");
		printk("ppid  : %d\n", cur->parent->pid);
		printk("root  : %s\n", dentry_path_raw(cur->fs->root.dentry, buffer, PATH_MAX));
		printk("pwd   : %s\n", dentry_path_raw(cur->fs->pwd.dentry, buffer, PATH_MAX));
		printk("\n");
	}

	return 0;
}

static void __exit hello_cleanup(void) {
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
