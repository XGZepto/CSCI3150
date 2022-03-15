#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/*	
	|-----------Group Information-----------|
		I don't have a group.
		This file is ZHOU, Yixiang's work.
		SID: 1155157100
	|---------------------------------------|
*/

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

int shell_execute(char ** args, int argc){

	if ( strcmp(args[0], "EXIT") == 0)
		return -1;
	
	int PIPE_FD[2];
	int INPUT_FD	= STDIN_FILENO;
	int OUTPUT_FD	= STDOUT_FILENO;

	int head = 0;
	int tail = 0;

	// head and tail indicate the position of each individual command
	while (head < argc - 1){

		while(tail < argc - 1 && (args + tail)[0][0] != '|') tail++;
		
		struct command *cmd = malloc(sizeof(struct command));
		cmd->argc = tail - head;
		cmd->argv = malloc(sizeof(char *) * (tail - head + 1));
		for (int i = 0; i < tail - head; i++)
			cmd->argv[i] = (args + head)[i];
		cmd->argv[tail - head] = NULL;

		if (tail == argc - 1){
			// if this is the last command, output to the terminal
			OUTPUT_FD = STDOUT_FILENO;
			spawn_command(INPUT_FD, OUTPUT_FD, cmd);
		} else {
			// otherwise create a new pipe and redirect the output to write side of the new pipe
			pipe(PIPE_FD);
			OUTPUT_FD = PIPE_FD[1];
			spawn_command(INPUT_FD, OUTPUT_FD, cmd);
			// remember to close for parent
			if (INPUT_FD != STDIN_FILENO) close(INPUT_FD);
			if (OUTPUT_FD != STDOUT_FILENO) close(OUTPUT_FD);
			// the input of the next command is the read side of the new pipe
			INPUT_FD = PIPE_FD[0];
		}
		
		free(cmd);

		if (tail >= argc - 1) break;
		if ((args + tail)[0][0] == '|') head = tail = tail + 1;
			else break;
	}

	// Piped commands run concurrently.
	pid_t wpid;
	int status = 0;
	while ((wpid = wait(&status)) > 0); // wait for all the child processes 

	return 0;
}