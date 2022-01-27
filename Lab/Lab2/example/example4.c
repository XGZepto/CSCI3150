#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
    char buf[20]="Hello World!";
    char rbuf1[20], rbuf2[20];
    int fd;
    if( (fd = open("myfile", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR )) < 0){
        printf("Error in open()\n");
        exit(-1);
    }
    if( write(fd, buf, 20) < 0){
        printf("Error in write()\n");
        exit(-1);
    }
    if( lseek(fd, 0, SEEK_SET) < 0){
        printf("Error in lseek()\n");
        exit(-1);
    }
    if( read(fd, rbuf1, 20) < 0){
        printf("Error in read()\n");
        exit(-1);
    }
    printf("rbuf1: %s\n", rbuf1);
    if( lseek(fd, 6, SEEK_SET) < 0){
        printf("Error in lseek()\n");
        exit(-1);
    }
    if( read(fd, rbuf2, 20) < 0){
        printf("Error in read()\n");
        exit(-1);
    }
    printf("rbuf2: %s\n", rbuf2);
    close(fd);
    return 0;
}