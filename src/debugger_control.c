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
  // printf("target started. will run '%s'\n", argv[0]);

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

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void dump_memory_region(FILE* pMemFile, unsigned long start_address, long length, int serverSocket)
{
    unsigned long address;
    int pageLength = 4096;
    unsigned char page[pageLength];
    fseeko(pMemFile, start_address, SEEK_SET);

    for (address=start_address; address < start_address + length; address += pageLength)
    {
        fread(&page, 1, pageLength, pMemFile);
          // write to stdout
          fwrite(&page, 1, pageLength, stdout);
    }
}

void dump_mem(pid_t child_pid)
{
       char mapsFilename[1024];
        sprintf(mapsFilename, "/proc/%d/maps", child_pid);
        FILE* pMapsFile = fopen(mapsFilename, "r");
        char memFilename[1024];
        sprintf(memFilename, "/proc/%d/mem", child_pid);
        FILE* pMemFile = fopen(memFilename, "r");

        char line[256];
        while (fgets(line, 256, pMapsFile) != NULL)
        {
            unsigned long start_address;
            unsigned long end_address;
            sscanf(line, "%08lx-%08lx\n", &start_address, &end_address);
            dump_memory_region(pMemFile, start_address, end_address - start_address, -1);
        }
        fclose(pMapsFile);
        fclose(pMemFile);
}

void run_debugger(pid_t child_pid, int *wait_status, struct options* prog_options) {

  unsigned icounter = 0;
  long long unsigned int ins_count = 0;
  long long unsigned int rax_is_zero = 0;
  long long unsigned int rcx_is_zero = 0;
  long long unsigned int rdx_is_zero = 0;
  long long unsigned int rdi_is_zero = 0;
  long long unsigned int rsi_is_zero = 0;

  /* Wait for child to stop on its first instruction */

  while (WIFSTOPPED(*wait_status)) {

    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
    unsigned long long instr = ptrace(PTRACE_PEEKTEXT, child_pid, regs.rip, 0);

    if (prog_options->trace_calls)
    {
      if (is_call(regs.rip, (char *)&instr) && addr_in_main_binary(regs.rip)) {
        printf("Call 0x%lX:\n", (long unsigned int)regs.rip);
        // dump_mem(child_pid);
        dump_regs(&regs, child_pid);
        icounter++;
      }
    }

    if (prog_options->reg_counts)
    {
      if (regs.rax == 0)
      {
        rax_is_zero ++;
      }
      if (regs.rcx == 0)
      {
        rcx_is_zero ++;
      }
      if (regs.rdx == 0)
      {
        rdx_is_zero ++;
      }
      if (regs.rdi == 0)
      {
        rdi_is_zero ++;
      }
      if (regs.rsi == 0)
      {
        rsi_is_zero ++;
      }
    }


    /* Make the child execute another instruction */
    if (ptrace(PTRACE_SINGLESTEP, child_pid, 0, 0) < 0) {
      log_and_exit("ptrace");
      return;
    }

    ins_count++;
    /* Wait for child to stop on its next instruction */
    wait(wait_status);
  }

  // printf("the child executed %u calls\n", icounter);
  if (prog_options->ins_counts)
  {
    printf("\n%llu : ins executed\n", ins_count);
  }
  
  if (prog_options->reg_counts)
  {
    printf("\n%llu : times RAX == 0\n", rax_is_zero);
    printf("\n%llu : times RCX == 0\n", rcx_is_zero);
    printf("\n%llu : times RDX == 0\n", rdx_is_zero);
    printf("\n%llu : times RDI == 0\n", rdi_is_zero);
    printf("\n%llu : times RSI == 0\n", rsi_is_zero);
  }

}