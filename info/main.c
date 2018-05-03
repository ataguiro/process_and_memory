#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/fs_struct.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adam Taguirov <ataguiro@student.42.fr>");
MODULE_DESCRIPTION("Hello World module");

struct pid_info {
	/*
	 * pid: pid of process
	 * name: name of the process
	 * state: unrunnable, runnable, stopped
	 * stack: pointer to the beginning of process's stack
	 * age: living time in nanoseconds
	 * child: array of all child processes pid
	 * ppid: parent process id
	 * root: root path of process
	 * pwd: working directory of process
	 */
	pid_t pid;
	char name[TASK_COMM_LEN];
	long state;
	void *stack;
	u64 age;
	pid_t child[256];
	pid_t ppid;
	char root_buf[PATH_MAX];
	char pwd_buf[PATH_MAX];
	char *root;
	char *pwd;
};

static int __init hello_init(void) {
	printk(KERN_INFO "Hello World !\n");
	struct task_struct *cur, *child;
	struct pid_info *new;
	struct path root, pwd;
	struct pid *spid;
	int i = 0;

	new = kmalloc(sizeof(struct pid_info), GFP_KERNEL);
	memset(new->child, 0, 256);
	spid = find_get_pid(1);
	cur = pid_task(spid, PIDTYPE_PID);
	get_fs_root(cur->fs, &root);
	get_fs_pwd(cur->fs, &pwd);
	
	get_task_comm(new->name, cur);
	new->pid = task_pid_nr(cur);	
	new->state = cur->state;
	new->stack = cur->stack;
	new->age = cur->start_time;
	list_for_each_entry(child, &cur->children, sibling) {
		if (i > 255)
			goto out;
		new->child[i++] = child->pid;
	}
out:
	new->ppid = task_pid_nr(cur->parent);

	spin_lock(&root.dentry->d_lock);
	new->root = dentry_path_raw(root.dentry, new->root_buf, PATH_MAX);
	spin_unlock(&root.dentry->d_lock);
	
	spin_lock(&pwd.dentry->d_lock);
	new->pwd = dentry_path_raw(pwd.dentry, new->pwd_buf, PATH_MAX);
	spin_unlock(&pwd.dentry->d_lock);

	printk("Printing struct pid_info...\n");
	printk("new->pid      : %d\n", new->pid);
	printk("new->name     : %s\n", new->name);
	printk("new->state    : %ld\n", new->state);
	printk("new->stack    : %p\n", new->stack);
	printk("new->age      : %lld\n", new->age);
	int j;
	for (j = 0; j < 255; j++)
	{
		if (!new->child[j])
			break ;
		printk("\tnew->child %d : %d\n", j, new->child[j]);
	}
	printk("new->ppid     : %d\n", new->ppid);
	printk("new->root     : %s\n", new->root);
	printk("new->pwd      : %s\n", new->pwd);

	return 0;	
}

static void __exit hello_cleanup(void) {
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
