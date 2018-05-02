#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/syscalls.h>

asmlinkage long sys_get_pid_info(struct pid_info *ret, int pid) {
	struct task_struct *cur, child;
	struct path root, pwd;
	struct pid *spid;
	char buffer[PATH_MAX], ptr[TASK_COMM_LEN];
	pid_t npid;

	spid = find_get_pid(pid);
	cur = pid_task(spid, PIDTYPE_PID);
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
	printk("root  : %s\n", dentry_path_raw(root.dentry, buffer, PATH_MAX));
	spin_unlock(&root.dentry->d_lock);
	spin_lock(&pwd.dentry->d_lock);
	printk("pwd   : %s\n", dentry_path_raw(pwd.dentry, buffer, PATH_MAX));
	spin_unlock(&pwd.dentry->d_lock);
	printk("\n");

	return 0;	
}
