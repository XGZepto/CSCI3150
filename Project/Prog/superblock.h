#ifndef _SUPER_BLOCK_H_
#define _SUPER_BLOCK_H_

#define SUPER_BLOCK_OFFSET  1024
#define INODE_OFFSET   2048
#define DATA_BLOCK_OFFSET    10485760
#define MAX_INODE      100
#define MAX_DATA_BLOCK   25600
#define BLOCK_SIZE   4096

#define MAX_COMMAND_LENGTH 50
#define MAX_FILE_SIZE BLOCK_SIZE*1024

typedef struct _super_block_
{
        int inode_offset;
        int data_block_offset;
        int max_inode;
        int max_data_block;
        int next_available_inode;
        int next_available_data_block;
        int block_size;
}superblock;

superblock* read_sb(int fd);
void print_sb_info(superblock* sb);
void print_sb_region(int fd);
#endif
