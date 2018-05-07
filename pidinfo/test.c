#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <inttypes.h>

#include <linux/module.h>
#include <linux/sched.h>

#include "struct_333.h"

#define TASK_COMM_LEN 16
#define PATH_MAX 4096

typedef uint64_t u64;

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
        char root[PATH_MAX];
        char pwd[PATH_MAX];
};


int main(int ac, char **av)
{
	pid_t pid;
	struct pid_info new;
	memset(&new, 0, sizeof(new));
	if (ac == 1)
		pid = getpid();
	else
		pid = atoi(av[1]);
	int ret = syscall(333, &new, pid);

	if (ret) {
		printf("syscall failed for one or other reason...\n");
		return 1;
	}
	printf("Printing struct pid_info...\n");
        printf("new->pid      : %d\n", new.pid);
        printf("new->name     : %s\n", new.name);
        printf("new->state    : %ld\n", new.state);
        printf("new->stack    : %llx\n", new.stack);
        printf("new->age      : %lld\n", new.age);
        int j;
        for (j = 0; j < 255; j++)
        {
                if (!new.child[j])
                        break ;
                printf("\tnew->child %d : %d\n", j, new.child[j]);
        }
        printf("new->ppid     : %d\n", new.ppid);
        printf("new->root     : %s\n", new.root);
        printf("new->pwd      : %s\n", new.pwd);
	return 0;
}
