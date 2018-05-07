#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <inttypes.h>

#include <linux/module.h>
#include <linux/sched.h>

#include "struct_333.h"

int main(int ac, char **av)
{
	pid_t pid;
	struct pid_info new;
	memset(&new, 0, sizeof(new));
	new.age = 0;
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
