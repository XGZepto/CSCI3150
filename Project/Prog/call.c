#include "call.h"
const char *HD = "HD";
#define min(a, b) ((a) < (b) ? (a) : (b))

// #define _DEBUG_
// uncomment to enable debug mode

void warning(char *msg){
	fprintf(stderr, "%s\n", msg);
}

void DEBUG_INFO(int offset, int inode_num, inode *cur_node){
	printf("DEBUG INFO\n");
	printf("offset: %d\n", offset);
	printf("inode_num: %d\n", cur_node->inode_number);
	printf("file_type: %d\n", cur_node->file_type);
	printf("file_size: %d\n", cur_node->file_size);
	printf("block_number: %d\n", cur_node->block_number);
	printf("direct_block[0]: %d\n", cur_node->direct_block[0]);
	printf("direct_block[1]: %d\n", cur_node->direct_block[1]);
	printf("indirect_block: %d\n", cur_node->indirect_block);
	printf("sub_file_number: %d\n", cur_node->sub_file_number);
	puts("");
}

int show_all_blocks(int inode_number){
	inode *cur_node = fetch_inode(inode_number);

	printf("inode info.\n");
	printf("inode_number: %d\n", cur_node->inode_number);
	printf("file_type: %d\n", cur_node->file_type);
	printf("file_size: %d\n", cur_node->file_size);
	printf("block_number: %d\n", cur_node->block_number);
	printf("direct_block: %d, %d\n", cur_node->direct_block[0], cur_node->direct_block[1]);
	printf("indirect_block: %d\n", cur_node->indirect_block);
	printf("sub_file_number: %d\n", cur_node->sub_file_number);

	int disk = open(HD, O_RDWR);

	if (cur_node->block_number >= 3){
		for (int i = 0; i + 3 <= cur_node->block_number; ++i){
			int indirect_block_number = fetch_indirect_block_number(cur_node->indirect_block, i);
			printf("%d %d ", i, indirect_block_number);
		}
	}
	puts("");
	close(disk);
	return 0;
}

inode *fetch_inode(int inode_num){
	int disk = open(HD, O_RDWR);
	int inode_offset = inode_num * sizeof(inode) + INODE_OFFSET;
	
	if (lseek(disk, inode_offset, SEEK_SET) == -1){
		close(disk);
		warning("lseek error\n");
		return NULL;
	}
	
	inode *cur_node = (inode*)malloc(sizeof(inode));
	read(disk, cur_node, sizeof(inode));

	#ifdef _DEBUG_
	DEBUG_INFO(inode_offset, inode_num, cur_node);
	#endif

	close(disk);
	return cur_node;
}

int fetch_data(int block, char* buf, int offset, int count){
	int disk = open(HD, O_RDWR);
	int block_offset = block * BLOCK_SIZE + DATA_BLOCK_OFFSET + offset;
	
	if (lseek(disk, block_offset, SEEK_SET) == -1){
		close(disk);
		warning("lseek error\n");
		return -1;
	}
	
	int res = read(disk, buf, count);
	close(disk);
	return res;
}

int fetch_indirect_block_number(int block, int number){
	int disk = open(HD, O_RDWR);
	int block_offset = block * BLOCK_SIZE + DATA_BLOCK_OFFSET + number * sizeof(int);
	
	if (lseek(disk, block_offset, SEEK_SET) == -1){
		close(disk);
		warning("lseek error\n");
		return -1;
	}
	
	int *indirect_block_number = (int*)malloc(sizeof(int));
	read(disk, indirect_block_number, sizeof(int));
	close(disk);
	return *indirect_block_number;
}

int directory_handlinig(inode *dir, char *target){
	if (!dir->file_type){
		warning("this is not a directory\n");
		return -1;
	}

	int disk = open(HD, O_RDWR);
	DIR_NODE *files = malloc(sizeof(DIR_NODE));
	char *buf = malloc(dir->file_size);

	if (dir->block_number >= 1){
		fetch_data(dir->direct_block[0], buf, 0, min(dir->file_size, BLOCK_SIZE));
	}
	
	if (dir->block_number >= 2){
		fetch_data(dir->direct_block[1], buf + BLOCK_SIZE, 0, min(dir->file_size - BLOCK_SIZE, BLOCK_SIZE));
	}

	if (dir->block_number >= 3){
		for (int i = 0; i + 3 <= dir->block_number; ++i){
			int indirect_block_number = fetch_indirect_block_number(dir->indirect_block, i);
			int buf_offset = i * BLOCK_SIZE + 2 * BLOCK_SIZE;
			fetch_data(indirect_block_number, buf + buf_offset, 0, min(dir->file_size - buf_offset, BLOCK_SIZE));
		}
	}

	int res = -1;

	for (int i = 0; i < dir->sub_file_number; ++i){
		int buf_offset = i * sizeof(DIR_NODE);
		memcpy(files, buf + buf_offset, sizeof(DIR_NODE));
		
		#ifdef _DEBUG_
		printf("FILE INFO\n");
		printf("file_name: %s\n", files->file_name);
		printf("inode_number: %d\n", files->inode_number);
		#endif

		if (strcmp(files->file_name, target) == 0){
			close(disk);
			res = files->inode_number;
			#ifndef _DEBUG_
			break;
			#endif
		}
	}

	close(disk);
	return res;
}

int open_t(char *pathname){
	int inode_number;
	int path_len = strlen(pathname);
	if (pathname[0] != '/'){
		warning("pathname must start with '/'\n");
		return -1;
	}

	int l = 1, r = 1;
	inode *cur_node = fetch_inode(0);

	while(l < path_len){
		while(pathname[r] != '/' && r < path_len){
			r++;
		}
		char *target = (char*)malloc(r - l + 1);
		memcpy(target, pathname + l, r - l);
		
		#ifdef _DEBUG_
		printf("target: %s\n", target);
		getchar();
		#endif
		
		target[r - l] = '\0';
		inode_number = directory_handlinig(cur_node, target);
		if (inode_number == -1){
			warning("file not found\n");
			return -1;
		}
		cur_node = fetch_inode(inode_number);
		l = r + 1;
		r = l;
	}

	inode_number = cur_node->inode_number;

	return inode_number;
}

int read_t(int inode_number, int offset, void *buf, int count){

	#ifdef _DEBUG_
	show_all_blocks(inode_number);
	#endif

	int read_bytes = 0;
	inode *cur_node = fetch_inode(inode_number);

	if (cur_node->file_type){
		warning("this is not a file\n");
		return -1;
	}

	count = min(count, cur_node->file_size - offset);

	int start_relative_block = offset / BLOCK_SIZE;
	int start_offset = offset % BLOCK_SIZE;
	int end_relative_block = (offset + count - 1) / BLOCK_SIZE;
	int block_number, block_offset = start_offset;

	for (int i = start_relative_block; i <= end_relative_block && read_bytes < count; ++i){
		if (i < 2){
			block_number = cur_node->direct_block[i];
		}
		else{
			block_number = fetch_indirect_block_number(cur_node->indirect_block, i - 2);
		}
		int read_count = min(count - read_bytes, BLOCK_SIZE - block_offset);
		read_count = fetch_data(block_number, buf + read_bytes, block_offset, read_count);
		read_bytes += read_count;
		block_offset = 0;
	}

	return read_bytes; 
}
