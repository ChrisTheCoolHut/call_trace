#include "debugger_control.h"
#include "program_control.h"
#include "register_control.h"
#include "util.h"
#include <stdbool.h>

int main(int argc, char **argv) {
  pid_t child_pid;
  int wait_status;

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
    run_debugger(child_pid, &wait_status);

  } else {
    log_and_exit("fork");
  }
  return 0;
}