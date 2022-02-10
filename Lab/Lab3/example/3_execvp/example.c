/* example.c 
Print process id and then execute program hello.c by using execvp()
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
      printf("PID of example.c = %d\n", getpid());
      char *args[] = {"hello", "c", "programming", NULL};
      execvp("./hello", args);
      printf("Back to example.c");
      return 0;
}