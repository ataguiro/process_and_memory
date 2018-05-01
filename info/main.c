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
	struct task_struct *proces;

	for_each_process(proces) {

		printk(
				"Process: %s\n \
				PID_Number: %ld\n \
				Process State: %ld\n \
				Priority: %ld\n \
				RT_Priority: %ld\n \
				Static Priority: %ld\n \
				Normal Priority: %ld\n", \
				proces->comm, \
				(long)task_pid_nr(proces), \
				(long)proces->state, \
				(long)proces->prio, \
				(long)proces->rt_priority, \
				(long)proces->static_prio, \
				(long)proces->normal_prio \
		      );


		if(proces->parent)
			printk(
					"Parent process: %s, \
					PID_Number: %ld", \
					proces->parent->comm, \
					(long)task_pid_nr(proces->parent) \
			      );

		printk("\n\n");

	}

	return 0;
}

static void __exit hello_cleanup(void) {
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
