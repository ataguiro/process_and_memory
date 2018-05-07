#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/fs_struct.h>

/*
struct pid_info {
	
	 * pid: pid of process
	 * name: name of the process
	 * state: unrunnable, runnable, stopped
	 * stack: pointer to the beginning of process's stack
	 * age: living time in nanoseconds
	 * child: array of all child processes pid
	 * ppid: parent process id
	 * root: root path of process
	 * pwd: working directory of process
	 
	pid_t pid;
	char name[TASK_COMM_LEN];
	long state;
	void *stack;
	u64 age;
	pid_t child[256];
	pid_t ppid;
	char root[PATH_MAX];
	char pwd[PATH_MAX];
};
*/

asmlinkage long sys_get_pid_info(struct pid_info *ret, int pid) {
	struct task_struct *cur, *child;
	struct pid_info *new;
	struct path root, pwd;
	struct pid *spid;
	struct timespec ts;
	char *tmp, buffer[PATH_MAX] = {0};
	int i = 0;

	new = kmalloc(sizeof(struct pid_info), GFP_KERNEL);
	if (!new)
		return -ENOMEM;
	memset(new->child, 0, 256);
	if (!(spid = find_get_pid(pid)))
		goto fail;
	cur = pid_task(spid, PIDTYPE_PID);
	if (!cur)
		goto fail;
	get_fs_root(cur->fs, &root);
	get_fs_pwd(cur->fs, &pwd);
	
	get_task_comm(new->name, cur);
	new->pid = task_pid_nr(cur);
	new->state = cur->state;
	new->stack = cur->stack;
	getnstimeofday(&ts);
	new->age = ts.tv_sec - cur->start_time;
	list_for_each_entry(child, &cur->children, sibling) {
		if (i > 255)
			goto out;
		new->child[i++] = child->pid;
	}
out:
	new->ppid = task_pid_nr(cur->parent);

	spin_lock(&root.dentry->d_lock);
	tmp = dentry_path_raw(root.dentry, buffer, PATH_MAX);
	strcpy(new->root, tmp);
	spin_unlock(&root.dentry->d_lock);
	
	spin_lock(&pwd.dentry->d_lock);
	tmp = dentry_path_raw(pwd.dentry, buffer, PATH_MAX);
	strcpy(new->pwd, tmp);
	spin_unlock(&pwd.dentry->d_lock);

	if (copy_to_user(ret, new, sizeof(struct pid_info)))
		goto fail;
	return 0;
fail:
	return -ESRCH;	
}

