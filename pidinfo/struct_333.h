#ifndef STRUCT_333
# define STRUCT_333

#define TASK_COMM_LEN 16
#define PATH_MAX 4096

typedef uint64_t u64;

struct pid_info {
        /*
         * pid: pid of process
         * name: name of the process
         * state: unrunnable, runnable, stopped
         * stack: pointer to the beginning of process's stack
         * age: birth time in nanoseconds
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

#endif
