CC=gcc

SimpleShell: simple-shell.o simple-execute.o
		$(CC) -o SimpleShell simple-shell.o simple-execute.o

simple-shell.o: simple-shell.c
		$(CC) -c -o simple-shell.o simple-shell.c

simple-execute.o: simple-execute.c
		$(CC) -c -o simple-execute.o simple-execute.c
		
clean:
	rm -f SimpleShell simple-shell.o simple-execute.o