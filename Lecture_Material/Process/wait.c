#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(void){
    int i;
    int n = 5;
    pid_t childpid;
    int status;
    pid_t waitreturn;

    for (int i = 1 ; i < n ; ++i)
        if ((childpid = fork()) > 0){
            printf("Forked. i = %d. \n", i);
            break;
        }

    waitreturn = wait(&status);
    
    printf("Wait returns %d; Exit status: %d\n", waitreturn, WEXITSTATUS(status));
    
    printf("My PID:%d, Parent_PID: %d\n", getpid(), getppid());

    sleep(1);

    return 0;
}