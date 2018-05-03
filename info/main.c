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
	struct task_struct *child;
	struct path root;
	struct path pwd;
	struct pid *pid;
	char buffer[PATH_MAX];
	char ptr[TASK_COMM_LEN];
	pid_t npid;

	pid = find_get_pid(1);
	cur = pid_task(pid, PIDTYPE_PID);
	npid = task_pid_nr(cur);
	get_fs_root(cur->fs, &root);
	get_fs_pwd(cur->fs, &pwd);
	get_task_comm(ptr, cur);
	
	printk("name  : %s\n", ptr);
	printk("pid   : %d\n", npid);
	printk("state : %ld\n", cur->state);
	printk("stack : %p\n", cur->stack);
	printk("age   : %llu\n", cur->start_time);
	list_for_each_entry(child, &cur->children, sibling) {
		printk("%d\n", child->pid);
	}
	printk("ppid  : %d\n", cur->parent->pid);
	spin_lock(&root.dentry->d_lock);
	buffer = dentry_path_raw(root.dentry, buffer, PATH_MAX);
	printk("ROOT  : %s\n", buffer);
	spin_unlock(&root.dentry->d_lock);
	spin_lock(&pwd.dentry->d_lock);
	buffer = dentry_path_raw(pwd.dentry, buffer, PATH_MAX);
	printk("PWD   : %s\n", buffer);
	spin_unlock(&pwd.dentry->d_lock);
	printk("\n");

	return 0;
}

static void __exit hello_cleanup(void) {
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
