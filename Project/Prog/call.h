#ifndef _CALL_H_
#define _CALL_H_
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <stdbool.h>
#include "superblock.h"
#include "inode.h"
#include <sys/stat.h>

int open_t(char *pathname);
int read_t(int inode_number, int offset, void *buf, int count);

#endif

