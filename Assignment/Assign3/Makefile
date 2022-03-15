CC=gcc
FLAG= -Wall

Scheduler: scheduler-exec.o scheduler-impl.o linkedlist.o queue.o
		$(CC) -o Scheduler scheduler-impl.o scheduler-exec.o linkedlist.o queue.o $(FLAG)

scheduler-exec.o: scheduler-exec.c
		$(CC) -c -o scheduler-exec.o scheduler-exec.c $(FLAG) 

scheduler-impl.o: scheduler-impl.c
		$(CC) -c -o scheduler-impl.o scheduler-impl.c  $(FLAG) 

queue.o: queue.c
		$(CC) -c -o queue.o queue.c $(FLAG) 

linkedlist.o: linkedlist.c
		$(CC) -c -o linkedlist.o linkedlist.c $(FLAG) 

clean:
		rm *.o & rm Scheduler