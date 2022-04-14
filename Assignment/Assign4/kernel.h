#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int KERNEL_SPACE_SIZE;
extern int VIRTUAL_SPACE_SIZE;
extern int PAGE_SIZE;
extern int MAX_PROCESS_NUM;

// The simple min function you can use in your codes.
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

/*
  Reference: textbook chapter 18.
  To make it simple, we do not encode PFN and flag bits together to an integer.
  PTE: page table entry.
  PFN: page frame number (here we use it to indicate the page id in kernel managed memory).

  present: represents if this translation is built, 0 -> not built, 1 -> built.
  Currently when the pages are allocated (proc_create_vm), PFN will be set to -1 and
  present will be set to 0 because the translation is not yet built.
  After you access this page (vm_read && vm_write), you will need to build the translation and present will be set to 1.
*/
struct PTE {
  int PFN;
  char present;
};

struct PageTable {
  struct PTE* ptes;
};

/*
  1. The user space and the user space page id start from 0.
  2. size indicates the size of user space (&& kernel-managed memory) allocated for this process.
  3. page_table is an array of PTE (page table entry).
*/
struct MMStruct {
  int size;
  struct PageTable* page_table;
};

// The Kernel manages MAX_PROCESS_NUM of processes.
struct Kernel {
  char* space;
  int allocated_pages;   // The number of allocated pages for processes.
  char* occupied_pages; // For simplicity, we use a char array to indicate the free pages, 0 for free, 1 for occupied.
  char* running;        // An array marking if the process is running.
  struct MMStruct* mm;  // An array of MMStruct for each process.
};

// Don't modify the content in the following 5 functions.
struct Kernel* init_kernel();
void destroy_kernel(struct Kernel* kernel);
void print_kernel_free_space(struct Kernel* kernel);
void get_kernel_free_space_info(struct Kernel* kernel, char* buf);
void print_memory_mappings(struct Kernel* kernel, int pid);

/*
  1. Check if a free process slot exists and if the there's enough free space (check allocated_pages).
  2. Alloc space for page_table (the size of it depends on how many pages you need) and update allocated_pages.
  3. The mapping to kernel-managed memory is not built up, all the PFN should be set to -1 and present byte to 0.
  4. Return a pid (the index in MMStruct array) which is >= 0 when success, -1 when failure in any above step.
*/
int proc_create_vm(struct Kernel* kernel, int size);

/*
  This function will read the range [addr, addr+size) from user space of a specific process to the buf (buf should be >= size).
  1. Check if the reading range is out-of-bounds.
  2. If the pages in the range [addr, addr+size) of the user space of that process are not present,
     you should firstly map them to the free kernel-managed memory pages (first fit policy).
  Return 0 when success, -1 when failure (out of bounds).
*/
int vm_read(struct Kernel* kernel, int pid, char* addr, int size, char* buf);

/*
  This function will write the content of buf to user space [addr, addr+size) (buf should be >= size).
  1. Check if the writing range is out-of-bounds.
  2. If the pages in the range [addr, addr+size) of the user space of that process are not present,
     you should firstly map them to the free kernel-managed memory pages (first fit policy).
  Return 0 when success, -1 when failure (out of bounds).
*/
int vm_write(struct Kernel* kernel, int pid, char* addr, int size, char* buf);

/*
  This function will free the space of a process.
  1. Reset the corresponding pages in occupied_pages to 0.
  2. Release the page_table in the corresponding MMStruct and set to NULL.
  Return 0 when success, -1 when failure.
*/
int proc_exit_vm(struct Kernel* kernel, int pid);