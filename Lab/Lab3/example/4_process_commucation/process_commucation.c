#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void)
{
        int pipefd[2];

        int rv= pipe(pipefd);
        assert(rv > -1);

        int cid = fork();
        assert(cid > -1);

        if( cid > 0 ){
                //Parent - Redirect stdout to the write end of the pipe, and execute "ls -l"
                close(1);
                dup(pipefd[1]);
                close(pipefd[0]);
                close(pipefd[1]);
                char *const ls_argv[] = {"ls", "-l", NULL};
execvp("ls", ls_argv);
        }else{
                //Child - Redirect stdin to the read end of the pipe, and execute "wc -l"
                close(0);
                dup(pipefd[0]);
                close(pipefd[0]);
                close(pipefd[1]);
                char *const wc_argv[] = {"wc", "-l", NULL};
                execvp("wc", wc_argv);
        }
}
