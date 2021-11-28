#include "register_control.h"
#include "debugger_control.h"
#include "util.h"

// Rdi, RSI, RDX, RCX
void dump_regs(struct user_regs_struct *regs, pid_t child_pid) {
  // If x86 arguments are on the stack
  // Pop off 4
  int i = 0;
  unsigned long protect_stack = 0;
  unsigned long stack_argument;
  unsigned long overflowed_buf = 0;
  unsigned long buf_chunk;
  unsigned int buf_chunk_int;
  unsigned long addr = 0;
  unsigned int extra_size = 0;
  char *stage_buf = malloc(1024);
  char *buf = malloc(1024);
  char *ptr = NULL;
  int word_size = 8;
  if (buf == NULL)
    log_and_exit("Failed to malloc\n");
    if (stage_buf == NULL)
    log_and_exit("Failed to malloc\n");
  memset(buf, '\x00', 1024);

  if (is_32_bit) {
    word_size = 4;

    for (i = 0; i < 4; i++) {
      extra_size = 0;
      stack_argument =
          ptrace(PTRACE_PEEKTEXT, child_pid, regs->rsp + (sizeof(int) * i), 0);
      if (is_32_bit)
        stack_argument &= 0xFFFFFFFF;

      if (addr_in_main_binary(stack_argument) ||
          addr_in_stack_or_heap(stack_argument)) {
        buf_chunk = ptrace(PTRACE_PEEKTEXT, child_pid, stack_argument, 0);
        if (is_32_bit)
          buf_chunk = buf_chunk & 0xFFFFFFFF;

        ptr = buf;
        while (addr_in_main_binary(buf_chunk) ||
            addr_in_stack_or_heap(buf_chunk)) {

          snprintf(stage_buf, 1024, "0x%lx -> ",buf_chunk);
          memcpy(ptr, stage_buf, strlen(stage_buf));
          ptr += strlen(stage_buf);
          extra_size += strlen(stage_buf);
          stack_argument = buf_chunk;
                  if (is_32_bit)
            buf_chunk = buf_chunk & 0xFFFFFFFF;
          buf_chunk = ptrace(PTRACE_PEEKTEXT, child_pid, stack_argument, 0);
          memset(stage_buf, '\x00',1024);
        }

        /*
         * While there is more printable character, keep
         * reading from the child process
         */
        while (strlen((char *)&buf_chunk) == word_size) {
          buf_chunk_int = buf_chunk;

          if (is_32_bit) {
            memcpy(ptr, &buf_chunk_int, word_size);
          } else {
            memcpy(ptr, &buf_chunk, word_size);
          }

          ptr += word_size;
          buf_chunk = ptrace(PTRACE_PEEKTEXT, child_pid,
                             stack_argument + (ptr - buf - extra_size), 0);
          if (is_32_bit) {
            buf_chunk = buf_chunk & 0xFFFFFFFF;
          }
        }
        memcpy(ptr, &buf_chunk, strlen((char *)&buf_chunk));

        printf("\t[0x%lx] 0x%lx -> ", stack_argument, stack_argument);
        print_alnum_string(buf);
        printf(" : [%lX]\n", (long unsigned int)buf);

        memset(buf, '\x00', 1024);
        ptr = buf;
      } else {
        printf("\t[0x%lx] 0x%lX\n", stack_argument, stack_argument);
      }
    }
  } else {
    // 64bit
    long long unsigned int *regs_dump = malloc(sizeof(&regs->rdi) * 4);
    if (regs_dump == NULL)
      log_and_exit("malloc failed\n");

    // First 4 argument registers
    regs_dump[0] = regs->rdi;
    regs_dump[1] = regs->rsi;
    regs_dump[2] = regs->rdx;
    regs_dump[3] = regs->rcx;
    char *names[4] = {"RDI", "RSI", "RDX","RCX"};
    for (i = 0; i < 4; i++) {
      extra_size = 0;
      addr = regs_dump[i];
      if (addr_in_main_binary(addr) ||
          addr_in_stack_or_heap(addr)) {
        buf_chunk = ptrace(PTRACE_PEEKTEXT, child_pid, addr, 0);
        ptr = buf;

        // While a valid ptr, keep dereferencing
        while (addr_in_main_binary(buf_chunk) ||
            addr_in_stack_or_heap(buf_chunk)) {

          snprintf(stage_buf, 1024, "0x%lx -> ",buf_chunk);
          memcpy(ptr, stage_buf, strlen(stage_buf));
          ptr += strlen(stage_buf);
          extra_size += strlen(stage_buf);
          addr = buf_chunk;
          buf_chunk = ptrace(PTRACE_PEEKTEXT, child_pid, addr, 0);
          memset(stage_buf, '\x00',1024);
        }

        
        /*
         * While there is more printable character, keep
         * reading from the child process
         */
        while (strlen((char *)&buf_chunk) >= word_size) {
          buf_chunk_int = buf_chunk;

          memcpy(ptr, &buf_chunk, word_size);

          ptr += word_size;
          buf_chunk =
              ptrace(PTRACE_PEEKTEXT, child_pid, addr + (ptr - buf - extra_size), 0);
        }
        memcpy(ptr, &buf_chunk, strlen((char *)&buf_chunk));

        printf("\t[%s] 0x%lx -> ", names[i], (long unsigned int)addr);
        print_alnum_string(buf);
        printf(" : [%lX]\n", (long unsigned int)buf);

        memset(buf, '\x00', 1024);
        ptr = buf;
      } else {
        printf("\t[%s] 0x%lX\n", names[i], (long unsigned int)addr);
      }
    }
  }
}