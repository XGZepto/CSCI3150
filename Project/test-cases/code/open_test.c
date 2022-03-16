#include "call.h"

int main (int argc, char *argv[])
{
	char filename[5][MAX_COMMAND_LENGTH] = {"/", "/dir5", "/dir5/dir1", "/dir5/dir1/file1", "/dir5/dir8/dir12/file2"};
	
	int expected[5] = {0, 1, 5, 13, 14};

	//Start testing
	for(int i = 0; i < 5; i++)
	{
		int inode_number = open_t(filename[i]);
		printf("======case %d: open \'%s\' =======\n", i, filename[i]);
		printf("returned inode number: %d\t expected result: %d\n\n", inode_number, expected[i]);
	}
	return 0;
}
