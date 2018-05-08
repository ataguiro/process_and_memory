#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>

#include <linux/module.h>
#include <linux/sched.h>

#include "struct_333.h"

void print_parents(pid_t pid)
{
	struct pid_info new;
	static index = 0;

        printf("\tParent %d : %d\n", index++, pid);
	
	if (!pid)
		return ;
	int ret = syscall(333, &new, pid);
	if (ret) {
                printf("syscall failed for one or other reason...\n");
                exit(EXIT_FAILURE);
        }
	print_parents(new.ppid);
}

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
		return EXIT_FAILURE;
	}
	printf("Printing struct pid_info...\n");
        printf("PID       : %d\n", new.pid);
        printf("Name      : %s\n", new.name);
        printf("State     : %ld\n", new.state);
        printf("Stack     : %llx\n", new.stack);
        printf("Birthtime : %lld\n", new.age);
        int j;
        for (j = 0; j < 255; j++)
        {
                if (!new.child[j])
                        break ;
                printf("\tChild %d  : %d\n", j, new.child[j]);
        }
	print_parents(new.ppid);
        printf("Root      : %s\n", new.root);
        printf("PWD       : %s\n", new.pwd);
	return EXIT_SUCCESS;
}
