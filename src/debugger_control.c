#include "debugger_control.h"
#include "register_control.h"
#include "util.h"

bool is_call(unsigned long location, char *code_bytes) {
  csh handle;
  cs_insn *insn;
  size_t count;
  bool is_call = false;

  if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK)
    log_and_exit("Failed to init capstone\n");

  count =
      cs_disasm(handle, code_bytes, sizeof(code_bytes) - 1, location, 0, &insn);

  if (count > 0) {
    size_t j;
    for (j = 0; j < count; j++) {
      if (strstr("call", insn[j].mnemonic)) {

        is_call = true;
        cs_free(insn, count);
        goto cleanup;
      }

      break;
    }

    cs_free(insn, count);
  }

cleanup:

  cs_close(&handle);
  return is_call;
}

void run_target(char **argv) {
  char *ptr;
  int i = 0;
  while (ptr != NULL) {
    ptr = argv[i + 1];
    argv[i] = ptr;
    i++;
  }
  printf("target started. will run '%s'\n", argv[0]);

  /* Allow tracing of this process */
  if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
    log_and_exit("ptrace");
    return;
  }

  /* Replace this process's image with the given program */
  execve(argv[0], argv, 0);

  log_and_exit("Shouldn't get here\n");
}

void print_alnum_string(char *str) {
  int i = 0;
  for (i = 0; i < strlen(str); i++) {
    if (str[i] >= 0x20 && str[i] <= 0x7F)
      printf("%c", str[i]);
  }
}

void run_debugger(pid_t child_pid, int *wait_status) {

  unsigned icounter = 0;
  printf("debugger started\n");

  /* Wait for child to stop on its first instruction */

  while (WIFSTOPPED(*wait_status)) {

    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
    unsigned long long instr = ptrace(PTRACE_PEEKTEXT, child_pid, regs.rip, 0);

    if (is_call(regs.rip, (char *)&instr) && addr_in_main_binary(regs.rip)) {
      printf("Call 0x%lX:\n", (long unsigned int)regs.rip);
      dump_regs(&regs, child_pid);
      icounter++;
    }

    /* Make the child execute another instruction */
    if (ptrace(PTRACE_SINGLESTEP, child_pid, 0, 0) < 0) {
      log_and_exit("ptrace");
      return;
    }

    /* Wait for child to stop on its next instruction */
    wait(wait_status);
  }

  printf("the child executed %u calls\n", icounter);
}