#include "util.h"
#include <stdbool.h>
#include <sys/types.h>

char *get_prog_path(pid_t pid);
void get_main_binary_addresses(char *prog_name, pid_t pid);
bool is_proc_32_bit(pid_t pid);