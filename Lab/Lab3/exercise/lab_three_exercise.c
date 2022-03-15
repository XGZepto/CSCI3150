#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

struct command{
	char **argv;
	int argc;
};

void spawn_command(int in, int out, struct command *cmd){
	pid_t child_pid;

	if((child_pid = fork()) < 0){
		perror("fork() error \n");
	}
	
	if (child_pid == 0 ){

		if (in != STDIN_FILENO){
			// should be replaced with dup2; same for the following
			close(STDIN_FILENO);
			dup(in);
			close(in);
		}

		if (out != STDOUT_FILENO){
			close(STDOUT_FILENO);
			dup(out);
			close(out);
		}

		if (execvp(cmd->argv[0], cmd->argv) < 0){
			perror("execvp() error");
			exit(1);
		}

	}
}

int main(void){

		struct command *cmd0 = malloc(sizeof(struct command));
		struct command *cmd1 = malloc(sizeof(struct command));
		struct command *cmd2 = malloc(sizeof(struct command));

		cmd0->argc = 3;
		cmd0->argv = malloc(sizeof(char *) * 3);
		cmd0->argv[0] = "ls";
		cmd0->argv[1] = "-l";
		cmd0->argv[2] = NULL;

		cmd1->argc = 3;
		cmd1->argv = malloc(sizeof(char *) * 3);
		cmd1->argv[0] = "grep";
		cmd1->argv[1] = "D";
		cmd1->argv[2] = NULL;

		cmd2->argc = 3;
		cmd2->argv = malloc(sizeof(char *) * 3);
		cmd2->argv[0] = "wc";
		cmd2->argv[1] = "-c";
		cmd2->argv[2] = NULL;


        int pipe1[2], pipe2[2];
        int rv1= pipe(pipe1);
        assert(rv1 > -1);
        int rv2= pipe(pipe2);
        assert(rv2 > -1);
        
		spawn_command(STDIN_FILENO, pipe1[1], cmd0);
		close(pipe1[1]);
		spawn_command(pipe1[0], pipe2[1], cmd1);
		close(pipe1[0]);
		close(pipe2[1]);
		spawn_command(pipe2[0], STDOUT_FILENO, cmd2);
		close(pipe2[0]);
		
		free(cmd0);
		free(cmd1);
		free(cmd2);

		pid_t wpid;
		int status = 0;
		while ((wpid = wait(&status)) > 0); // wait for all the child processes
}