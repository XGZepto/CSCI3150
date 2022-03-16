#ifndef _INODE_H_
#define _INODE_H_
#include <time.h>

#define FILE 0
#define DIR 1

typedef struct _inode_ 
{
	int inode_number;
	time_t creation_time;
	int file_type;
	int file_size;
	int block_number;
	int direct_block[2];
	int indirect_block;
	int sub_file_number;
}inode;

typedef struct directory_mapping
{
	char file_name[20];
	int inode_number;
}DIR_NODE;

inode* read_inode(int fd, int inode_number);
void print_inode_info(inode* ip);
void print_inode_region(int fd, int inode_number);
void print_directory_mappings(int fd, int inode_number);
#endif
