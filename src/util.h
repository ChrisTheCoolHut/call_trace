#ifndef CALL_TRACE_UTIL
#define CALL_TRACE_UTIL
#include <elf.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#include <capstone/capstone.h>

extern unsigned long main_bin_start_addr;
extern unsigned long main_bin_end_addr;

extern unsigned long stack_start_addr;
extern unsigned long stack_end_addr;

extern unsigned long heap_start_addr;
extern unsigned long heap_end_addr;
extern bool is_32_bit;

void log_and_exit(char *msg, ...);

bool addr_in_main_binary(unsigned long addr);
bool addr_in_stack_or_heap(unsigned long addr);

#endif /* CALL_TRACE_UTIL */
