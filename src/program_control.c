#include "program_control.h"
#include "util.h"

char *get_prog_path(pid_t pid) {
  char *path = malloc(PATH_MAX);
  if (path == NULL)
    log_and_exit("Failed to malloc\n");
  char *buf = malloc(PATH_MAX);
  if (buf == NULL)
    log_and_exit("Failed to malloc\n");

  snprintf(buf, PATH_MAX, "/proc/%d/exe", pid);

  if (path != NULL) {
    if (readlink(buf, path, PATH_MAX) == -1) {
      free(path);
      path = NULL;
    }
  }
  free(buf);
  return path;
}

void get_main_binary_addresses(char *prog_name, pid_t pid) {
  FILE *fp;
  char *line = NULL;
  char *ptr = NULL;
  size_t len = 0;
  ssize_t read;
  unsigned long long read_val = 0;
  char *buf = malloc(PATH_MAX);
  if (buf == NULL)
    log_and_exit("Failed to malloc\n");

  snprintf(buf, PATH_MAX, "/proc/%d/maps", pid);

  fp = fopen(buf, "r");
  if (fp == NULL)
    log_and_exit("Failed to open /proc/self/maps");
  // 557251dbf000-557251dc0000 r--p 00000000 103:02 59507082
  while ((read = getline(&line, &len, fp)) != -1) {
    if (strstr(line, prog_name)) {

      read_val = strtoul(line, &ptr, 16);
      if (main_bin_start_addr == 0) {
        main_bin_start_addr = read_val;
      }
      // Increment past -
      ptr++;

      main_bin_end_addr = strtoul(ptr, NULL, 16);
    }
    if (strstr(line, "stack")) {

      read_val = strtoul(line, &ptr, 16);
      if (stack_start_addr == 0) {
        stack_start_addr = read_val;
      }
      // Increment past -
      ptr++;

      stack_end_addr = strtoul(ptr, NULL, 16);
    }
    if (strstr(line, "heap")) {

      read_val = strtoul(line, &ptr, 16);
      if (heap_start_addr == 0) {
        heap_start_addr = read_val;
      }
      // Increment past -
      ptr++;

      heap_end_addr = strtoul(ptr, NULL, 16);
    }
  }

  fclose(fp);
  if (line)
    free(line);
  printf("[~] Binary is mapped from 0x%lx to 0x%lx\n", main_bin_start_addr,
         main_bin_end_addr);
}

bool is_proc_32_bit(pid_t pid) {
  char bitness = 0;
  char *buf = malloc(1024);
  int fd;
  if (buf == NULL)
    log_and_exit("Failed to malloc\n");

  snprintf(buf, PATH_MAX, "/proc/%d/exe", pid);

  fd = open(buf, O_RDONLY);
  if (fd < 0)
    log_and_exit("Couldn't open /proc/pid/exe\n");

  lseek(fd, 4, SEEK_SET);

  read(fd, &bitness, 1);

  close(fd);

  return bitness == 1;
}