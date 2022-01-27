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
	|---------------------------------------|
*/

struct command{
	char **argv;
	int argc;
};

int spawn_command(int in, int out, struct command *cmd){
	pid_t child_pid;
	int wait_return, status;

	if((child_pid = fork()) < 0){
		perror("fork() error \n");
		return -1;
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

	if ( (wait_return = wait(&status) ) < 0 )
		perror("wait() error");
	return status;
}

#define MAX_LINE_SIZE 1024   //the maximum bytes of an inputted command line

void flush_pipe(int in, int out){
	int seg_size = 0;
	char *buffer = malloc(MAX_LINE_SIZE * sizeof(char));
	do{
		seg_size = read(in, buffer, MAX_LINE_SIZE);
		write(out, buffer, seg_size);
	}while(seg_size == MAX_LINE_SIZE);
	free(buffer);
}

#undef MAX_LINE_SIZE

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

		// if no pipe is needed, execute the command directly
		if (head == 0 && tail == argc - 1){
			spawn_command(INPUT_FD, OUTPUT_FD, cmd);
			free(cmd);
			return 0;
		}

		// create a new pipe and redirect the output to write side of the new pipe
		pipe(PIPE_FD);
		OUTPUT_FD = PIPE_FD[1];
		int return_status = spawn_command(INPUT_FD, OUTPUT_FD, cmd);

		// the input of the next command is the read side of the new pipe
		INPUT_FD = PIPE_FD[0];

		// close the write side of this pipe, we will not use it anymore
		close(PIPE_FD[1]);
		free(cmd);

		if (tail >= argc - 1) break;
		if ((args + tail)[0][0] == '|') head = tail = tail + 1;
			else break;
	}

	// flush the content of the last pipe to the standard output and closer the read side of this pipe.
	flush_pipe(PIPE_FD[0], STDOUT_FILENO);
	close(PIPE_FD[0]);
	
	return 0;
}