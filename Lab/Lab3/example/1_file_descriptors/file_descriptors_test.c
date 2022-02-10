#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
        int fd = open("myfile", O_WRONLY | O_CREAT| O_TRUNC, S_IRWXU);
        assert(fd > -1);
        printf("File descriptor for myfile - %d\n", fd);

        char buf[10];

        int rc = read(0, buf, 10);  //File descriptor 0 is stdin
        assert(rc <= 10);

        int wc = write(1, buf, rc);  //File descriptor 1 is stdout
        assert( wc == rc);
        close(fd);
        return 0;
}
