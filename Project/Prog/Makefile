CC = gcc
CFLAGS = -O -m32

open_test: open_test.o call.o
	${CC} -o open_test open_test.o call.o ${CFLAGS}

read_test: read_test.o call.o
	${CC} -o read_test read_test.o call.o ${CFLAGS}

clean:
	rm -f *.o & rm -f open_test & rm -f read_test