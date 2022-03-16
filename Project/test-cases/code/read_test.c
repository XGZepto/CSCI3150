#include "call.h"

int main (int argc, char *argv[])
{
	//argv[1]= A new file in SFS with full pathname
	char filename[MAX_COMMAND_LENGTH]="/dir5/dir1/file1";

	/*
	Allocate a buf with MAX_FILE_SIZE.
	*/
	char buf[MAX_FILE_SIZE];

	int read_size;
	int test_inode=open_t(filename);
	//Start test
	//The size of Plato.txt
	int file_size = 1239081;
	int offset_list[10] = {0,   4100, 8500,  40965, 15,   100,   9000,  file_size - 50, file_size + 50, 10};
	int count_list[10] =  {100, 1000, 300,   800,   5000, 50000, 60000, 10000,        10,           MAX_FILE_SIZE - 100};
	int expected[10] = {100, 1000, 300, 800, 5000, 50000, 60000, 50, 0, file_size -10};
	//read_t test
	for(int i = 0; i < 10; i++)
	{
		int cnt = count_list[i];
		int off = offset_list[i];
		printf("====case %d: read %d bytes from %d offest=======\n", i, cnt, off);
		read_size = read_t(test_inode, off, buf, cnt);
		buf[read_size] = '\0';
		printf("read size: %d\t expected: %d\n\n",read_size, expected[i]);
		printf("read bytes: %s\n\n",buf);
	}
	return 0;
}