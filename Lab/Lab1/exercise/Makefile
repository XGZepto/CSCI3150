CC = gcc
OBJS = main.o foo.o bar.o 
TARGET = lab1
CFLAGS = -Wall -O

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${CFLAGS}

foo.o: ./foo/foo.c
	${CC} -c ./foo/foo.c

bar.o: ./bar/bar.c
	${CC} -c ./bar/bar.c

clean:
	rm -f *.o