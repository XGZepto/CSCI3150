#include "kernel.h"

/*
  1. Check if a free process slot exists and if the there's enough free space (check allocated_pages).
  2. Alloc space for page_table (the size of it depends on how many pages you need) and update allocated_pages.
  3. The mapping to kernel-managed memory is not built up, all the PFN should be set to -1 and present byte to 0.
  4. Return a pid (the index in MMStruct array) which is >= 0 when success, -1 when failure in any above step.
*/
int proc_create_vm(struct Kernel* kernel, int size){
	
	if (size > VIRTUAL_SPACE_SIZE) return -1;

	int pid = -1;
	for (int i = 0; i < MAX_PROCESS_NUM; ++i)
		if (!kernel->running[i]){
			pid = i;
			break;
		}

	if (pid == -1) return -1;

	int pages = (size - 1) / PAGE_SIZE + 1;
	if (kernel->allocated_pages + pages > KERNEL_SPACE_SIZE / PAGE_SIZE) return -1;

	kernel->allocated_pages += pages;
	kernel->running[pid] = 1;
	kernel->mm[pid].size = size;
	kernel->mm[pid].page_table = malloc(sizeof(struct PageTable));
	kernel->mm[pid].page_table->ptes = malloc(sizeof(struct PTE) * pages);

	for (int i = 0; i < pages; ++i){
		kernel->mm[pid].page_table->ptes[i].PFN = -1;
		kernel->mm[pid].page_table->ptes[i].present = 0;
	}

	return pid;
}

/*
  This function will read the range [addr, addr+size) from user space of a specific process to the buf (buf should be >= size).
  1. Check if the reading range is out-of-bounds.
  2. If the pages in the range [addr, addr+size) of the user space of that process are not present,
     you should firstly map them to the free kernel-managed memory pages (first fit policy).
  Return 0 when success, -1 when failure (out of bounds).
*/
int vm_read(struct Kernel* kernel, int pid, char* addr, int size, char* buf){
	if ((int)addr + size > kernel->mm[pid].size) return -1;

	int hd_page 	= (int)addr / PAGE_SIZE;
	int hd_offset 	= (int)addr % PAGE_SIZE;
	int tl_page 	= (int)(addr + size - 1) / PAGE_SIZE;
	int tl_offset 	= (int)(addr + size) % PAGE_SIZE;

	for (int i = hd_page, cur = 0; i <= tl_page; ++i){
		if (!kernel->mm[pid].page_table->ptes[i].present){
			int free_page = -1;
			for (int j = 0; j < KERNEL_SPACE_SIZE / PAGE_SIZE; ++j)
				if (!kernel->occupied_pages[j]){
					free_page = j;
					break;
				}

			if (free_page == -1) return -1;

			kernel->occupied_pages[free_page] = 1;
			kernel->mm[pid].page_table->ptes[i].PFN = free_page;
			kernel->mm[pid].page_table->ptes[i].present = 1;
		}

		int phys_page = kernel->mm[pid].page_table->ptes[i].PFN;

		if (hd_page == tl_page){
			memcpy(buf, kernel->space + phys_page * PAGE_SIZE + hd_offset, size);
			return 0;
		} else if (hd_page == i){
			memcpy(buf + cur, kernel->space + phys_page * PAGE_SIZE + hd_offset, PAGE_SIZE - hd_offset);
			cur += PAGE_SIZE - hd_offset;
		} else if (tl_page == i){
			memcpy(buf + cur, kernel->space + phys_page * PAGE_SIZE, tl_offset);
			cur += tl_offset;
		} else {
			memcpy(buf + cur, kernel->space + phys_page * PAGE_SIZE, PAGE_SIZE);
			cur += PAGE_SIZE;
		}

	}

	return 0;
}

/*
  This function will write the content of buf to user space [addr, addr+size) (buf should be >= size).
  1. Check if the writing range is out-of-bounds.
  2. If the pages in the range [addr, addr+size) of the user space of that process are not present,
     you should firstly map them to the free kernel-managed memory pages (first fit policy).
  Return 0 when success, -1 when failure (out of bounds).
*/
int vm_write(struct Kernel* kernel, int pid, char* addr, int size, char* buf){
	if ((int)addr + size > kernel->mm[pid].size) return -1;

	int hd_page 	= (int)addr / PAGE_SIZE;
	int hd_offset 	= (int)addr % PAGE_SIZE;
	int tl_page 	= (int)(addr + size - 1) / PAGE_SIZE;
	int tl_offset 	= (int)(addr + size) % PAGE_SIZE;

	for (int i = hd_page, cur = 0; i <= tl_page; ++i){
		if (!kernel->mm[pid].page_table->ptes[i].present){
			int free_page = -1;
			for (int j = 0; j < KERNEL_SPACE_SIZE / PAGE_SIZE; ++j)
				if (!kernel->occupied_pages[j]){
					free_page = j;
					break;
				}

			if (free_page == -1) return -1;

			kernel->occupied_pages[free_page] = 1;
			kernel->mm[pid].page_table->ptes[i].PFN = free_page;
			kernel->mm[pid].page_table->ptes[i].present = 1;
		}

		int phys_page = kernel->mm[pid].page_table->ptes[i].PFN;

		if (hd_page == tl_page){
			memcpy(kernel->space + phys_page * PAGE_SIZE + hd_offset, buf, size);
			return 0;
		} else if (hd_page == i){
			memcpy(kernel->space + phys_page * PAGE_SIZE + hd_offset, buf + cur, PAGE_SIZE - hd_offset);
			cur += PAGE_SIZE - hd_offset;
		} else if (tl_page == i){
			memcpy(kernel->space + phys_page * PAGE_SIZE, buf + cur, tl_offset);
			cur += tl_offset;
		} else {
			memcpy(kernel->space + phys_page * PAGE_SIZE, buf + cur, PAGE_SIZE);
			cur += PAGE_SIZE;
		}
	}

	return 0;
}

/*
  This function will free the space of a process.
  1. Reset the corresponding pages in occupied_pages to 0.
  2. Release the page_table in the corresponding MMStruct and set to NULL.
  Return 0 when success, -1 when failure.
*/
int proc_exit_vm(struct Kernel* kernel, int pid){
	if (!kernel->running[pid]) return -1;

	int pages = (kernel->mm[pid].size - 1) / PAGE_SIZE + 1;

	for (int i = 0; i < pages; ++i)
		if (kernel->mm[pid].page_table->ptes[i].present)
			kernel->occupied_pages[kernel->mm[pid].page_table->ptes[i].PFN] = 0;

	free(kernel->mm[pid].page_table->ptes);
	free(kernel->mm[pid].page_table);

	kernel->mm[pid].page_table = NULL;
	kernel->running[pid] = 0;
	kernel->allocated_pages -= pages;

	return 0;
}
