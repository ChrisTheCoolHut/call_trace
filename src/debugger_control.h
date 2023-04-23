#include <stdbool.h>
#include <sys/types.h>

struct options {
    bool trace_calls;
    bool reg_counts;
    bool ins_counts;
};

bool is_call(unsigned long location, char *code_bytes);

void run_target(char **argv);

void run_debugger(pid_t child_pid, int *wait_status, struct options* prog_options);

void print_alnum_string(char *str);