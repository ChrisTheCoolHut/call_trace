#include "debugger_control.h"
#include "program_control.h"
#include "register_control.h"
#include "util.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define NO_TRACE "NO_CALL_TRACE"
#define NO_REG_COUNTS "NO_REG_COUNTS"
#define NO_INS_COUNTS "NO_INS_TRACE"

int main(int argc, char **argv) {
  pid_t child_pid;
  int wait_status;

  struct options* prog_options = malloc(sizeof(struct options));
  prog_options->trace_calls = true;
  prog_options->reg_counts = true;
  prog_options->ins_counts = true;

  if (getenv(NO_TRACE)) 
  {
    printf("[~] NO_CALL_TRACE is set\n");
    prog_options->trace_calls = false;
  }
  
  if (getenv(NO_REG_COUNTS))
  {
    printf("[~] NO_REG_COUNTS is set\n");
    prog_options->reg_counts = false;
  }
   
  if (getenv(NO_INS_COUNTS))
  {
    printf("[~] NO_INS_COUNTS is set\n");
    prog_options->ins_counts = false;
  } 

  if (argc < 2) {
    log_and_exit("[-] Usage %s <program> [program args]\n", argv[0]);
  }

  child_pid = fork();
  if (child_pid == 0)
    run_target(argv);
  else if (child_pid > 0) {
    wait(&wait_status);
    if (is_proc_32_bit(child_pid))
      is_32_bit = true;
    char *prog_name = get_prog_path(child_pid);
    get_main_binary_addresses(prog_name, child_pid);
    printf("addr : %lx\n", main_bin_end_addr);
    run_debugger(child_pid, &wait_status, prog_options);

  } else {
    log_and_exit("fork");
  }
  return 0;
}