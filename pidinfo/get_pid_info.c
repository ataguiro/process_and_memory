#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/fs_struct.h>

#include "struct_333.h"

asmlinkage long sys_get_pid_info(struct pid_info *ret, int pid) {
	struct task_struct *cur, *child;
	struct pid_info *new;
	struct path root, pwd;
	struct pid *spid;
	/* struct timespec ts;
	 * struct sysinfo sys_info;
	 */
	 char *tmp, buffer[PATH_MAX] = {0};
	int i = 0;
	/* u64 tmp_time; */

	if (!(spid = find_get_pid(pid)))
		goto fail;
	cur = pid_task(spid, PIDTYPE_PID);
	if (!cur)
		goto fail;
	new = kmalloc(sizeof(struct pid_info), GFP_KERNEL);
	if (!new)
		return -ENOMEM;
	memset(new->child, 0, 256 * sizeof(pid_t));
	get_fs_root(cur->fs, &root);
	get_fs_pwd(cur->fs, &pwd);
	
	get_task_comm(new->name, cur);
	new->pid = task_pid_nr(cur);
	new->state = cur->state;
	new->stack = cur->stack;
	/* getnstimeofday(&ts);
	 * tmp_time = local_clock();
	 * sys_sysinfo(&sys_info);
	 * tmp_time = sys_info.uptime * 1000000000L;
	 */
	new->age = cur->start_time;
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

	if (copy_to_user(ret, new, sizeof(struct pid_info))) {
		kfree(new);
		goto fail;
	}
	kfree(new);
	return 0;
fail:
	return -ESRCH;	
}

