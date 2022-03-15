#include "superblock.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#define HD "./HD"

superblock* read_sb(int fd){
	superblock* sb = malloc(sizeof(superblock));
	int currpos=lseek(fd, SUPER_BLOCK_OFFSET, SEEK_SET);
	if(currpos<0){
		printf("Error: lseek()\n");
		return NULL;
	}
	
	//read superblock from disk
	int ret = read(fd, sb, sizeof(superblock));
	if(ret != sizeof (superblock) ){
		printf("Error: read()\n");
		return NULL;
	}
	return sb;
}

void print_sb_info(superblock* sb){
	printf("The superblock information:\n");
	printf("[1] inode_offset:\t\t%d\n", sb->inode_offset);
	printf("[2] data_block_offset:\t\t%d\n", sb->data_block_offset);
	printf("[3] max_inode:\t\t\t%d\n", sb->max_inode);
	printf("[4] max_data_block:\t\t%d\n", sb->max_data_block);
	printf("[5] next_available_inode:\t%d\n", sb->next_available_inode);
	printf("[6] next_available_data_block:\t%d\n", sb->next_available_data_block);
	printf("[7] block_size:\t\t\t%d\n", sb->block_size);
}

void print_sb_region(int fd){
	printf("The superblock region on disk (Hexadecimal):\n");
	unsigned int buf[sizeof(superblock) / 4];
	int currpos=lseek(fd, SUPER_BLOCK_OFFSET, SEEK_SET);
	read(fd, buf, sizeof(superblock));
	int i;
	for (i = 0; i < sizeof(superblock) / 4; i++){
		printf("[%d] %04x\n", i+1, buf[i]);
	}
}

