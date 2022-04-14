#include "kernel.h"

int KERNEL_SPACE_SIZE = 8192;
int VIRTUAL_SPACE_SIZE = 512;
int PAGE_SIZE = 32;
int MAX_PROCESS_NUM = 8;

// The kernel managed memory content is set to 0 initiallly.
struct Kernel* init_kernel() {
  struct Kernel* kernel = (struct Kernel*)malloc(sizeof(struct Kernel));

  kernel->space = (char*)malloc(sizeof(char) * KERNEL_SPACE_SIZE);
  kernel->allocated_pages = 0;
  kernel->occupied_pages = (char*)malloc(sizeof(char) * KERNEL_SPACE_SIZE / PAGE_SIZE);
  kernel->running = (char*)malloc(sizeof(char) * MAX_PROCESS_NUM);
  kernel->mm = (struct MMStruct*)malloc(sizeof(struct MMStruct) * MAX_PROCESS_NUM);

  for (int i = 0; i < MAX_PROCESS_NUM; i ++)
    kernel->mm[i].page_table = NULL;

  memset(kernel->space, 0, sizeof(char) * KERNEL_SPACE_SIZE);
  memset(kernel->occupied_pages, 0, sizeof(char) * KERNEL_SPACE_SIZE / PAGE_SIZE);
  memset(kernel->running, 0, sizeof(char) * MAX_PROCESS_NUM);

  return kernel;
}

void destroy_kernel(struct Kernel* kernel) {
  free(kernel->space);
  free(kernel->occupied_pages);
  free(kernel->running);
  for (int i = 0; i < MAX_PROCESS_NUM; i ++) {
    if (kernel->mm[i].page_table != NULL) {
      free(kernel->mm[i].page_table->ptes);
      free(kernel->mm[i].page_table);
    }
  }
  free(kernel->mm);
  free(kernel);
}

// Print the free kernel space.
void print_kernel_free_space(struct Kernel* kernel) {
  int idx = 0;
  char* addr = kernel->space;
  printf("free space: ");
  while (idx < KERNEL_SPACE_SIZE / PAGE_SIZE) {
    while (idx < KERNEL_SPACE_SIZE / PAGE_SIZE && kernel->occupied_pages[idx] == 1) {
      ++ idx;
      addr += PAGE_SIZE;
    }
    int last = idx;
    while (idx < KERNEL_SPACE_SIZE / PAGE_SIZE && kernel->occupied_pages[idx] == 0)
      ++ idx;
    if (idx < KERNEL_SPACE_SIZE / PAGE_SIZE)
      printf("(addr:%d, size:%d) -> ", (int)(addr - kernel->space), (idx - last) * PAGE_SIZE);
    else
      printf("(addr:%d, size:%d)\n", (int)(addr - kernel->space), (idx - last) * PAGE_SIZE);
    addr += PAGE_SIZE * (idx - last);
  }
}

// Copy the free kernel free space information to buf.
void get_kernel_free_space_info(struct Kernel* kernel, char* buf) {
  int i = sprintf(buf, "free space: ");
  int idx = 0;
  char* addr = kernel->space;
  while (idx < KERNEL_SPACE_SIZE / PAGE_SIZE) {
    while (idx < KERNEL_SPACE_SIZE / PAGE_SIZE && kernel->occupied_pages[idx] == 1) {
      ++ idx;
      addr += PAGE_SIZE;
    }
    int last = idx;
    while (idx < KERNEL_SPACE_SIZE / PAGE_SIZE && kernel->occupied_pages[idx] == 0)
      ++ idx;
    int n;
    if (idx < KERNEL_SPACE_SIZE / PAGE_SIZE)
      n = sprintf(buf + i, "(addr:%d, size:%d) -> ", (int)(addr - kernel->space), (idx - last) * PAGE_SIZE);
    else
      n = sprintf(buf + i, "(addr:%d, size:%d)\n", (int)(addr - kernel->space), (idx - last) * PAGE_SIZE);
    i += n;
    addr += PAGE_SIZE * (idx - last);
  }
}

// Print memory mappings for a specific process.
void print_memory_mappings(struct Kernel * kernel, int pid) {
  if (kernel->running[pid] == 0) {
    printf("The process is not running\n");
  }
  else {
    printf("Memory mappings of process %d\n", pid);
    for (int i = 0; i < (kernel->mm[pid].size + PAGE_SIZE - 1) / PAGE_SIZE; i++) {
      if (kernel->mm[pid].page_table->ptes[i].present == 0)
        printf("virtual page %d: Not present\n", i);
      else
        printf("virtual page %d -> physical page %d\n", i, kernel->mm[pid].page_table->ptes[i].PFN);
    }
  }
  printf("\n");
}