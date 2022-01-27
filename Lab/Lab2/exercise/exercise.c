#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void Error(char *msg) {
  perror(msg);
  exit(1);
}

int FetchInt(int fd){
    int res = 0, flag = 1;
    char buff[1];
    while(read(fd, buff, 1) > 0){
        if(buff[0] == '\n')
            break;
        if(buff[0] == '-'){
            flag = -1;
            continue;
        }
        else res = (res << 3) + (res << 1) + (buff[0] - '0');
    }
    return res * flag;
}

void FeedInt(int fd, int val){
    char buff[10];
    sprintf(buff, "%d\n", val);
    if (write(fd, buff, strlen(buff)) < 0)
        Error("Error in write()");
}

int main(){
    int inputfile = open("input.txt", O_RDONLY);
    int outputfile = open("output.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (inputfile < 0) Error("Error in opening input file\n");
    if (outputfile < 0) Error("Error in opening output file\n");

    int *arr = (int *)malloc(sizeof(int) * 10);
    for (int i = 0; i < 10; ++i)
        arr[i] = FetchInt(inputfile);

    for (int i = 0; i < 10; ++i)
        FeedInt(outputfile, arr[i] + 1);

    close(inputfile);
    close(outputfile);
    return 0;
}