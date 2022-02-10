/* hello.c 
Print a sentence “We are in Hello.c” and process id
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    printf("We are in hello.c\n");
    printf("PID of hello.c = %d\n", getpid());
    return 0;
}
