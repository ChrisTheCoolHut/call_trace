#include "util.h"

unsigned long main_bin_start_addr;
unsigned long main_bin_end_addr;

unsigned long stack_start_addr;
unsigned long stack_end_addr;

unsigned long heap_start_addr;
unsigned long heap_end_addr;
bool is_32_bit;

void log_and_exit(char *msg, ...) {
  va_list argList;

  va_start(argList, msg);
  vprintf(msg, argList);
  va_end(argList);
  exit(0);
}

bool addr_in_main_binary(unsigned long addr) {
  return addr > main_bin_start_addr && addr < main_bin_end_addr;
}
bool addr_in_stack_or_heap(unsigned long addr) {
  return (addr > stack_start_addr && addr < stack_end_addr) ||
         (addr > heap_start_addr && addr < heap_end_addr);
}