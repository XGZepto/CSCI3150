#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void)
{
        int fd = open("myfile", O_WRONLY | O_CREAT| O_TRUNC, S_IRWXU);
        assert(fd > -1);

        close(1);     // Close “stdout – standard output” – 1 becomes the unused file descriptor
        dup(fd);      // Duplicate “fd” to the lowest-numbered unusual file descriptor that is 1         
        close(fd);

        char *const ls_argv[] = {"ls", "-l", NULL };
        execvp("ls", ls_argv);
        return 0;
}