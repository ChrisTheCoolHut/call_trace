#include <stdbool.h>
#include <sys/types.h>

bool is_call(unsigned long location, char *code_bytes);

void run_target(char **argv);

void run_debugger(pid_t child_pid, int *wait_status);

void print_alnum_string(char *str);