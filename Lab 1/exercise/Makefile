CC = gcc
CFLAGS = -Wall
OBJS = main.o foo.o bar.o

main: ${OBJS}
	${CC} -o main main.o foo.o bar.o

foo.o: ./foo/foo.c
	${CC} -c ./foo/foo.c

bar.o: ./bar/bar.c
	${CC} -c ./bar/bar.c

clean:
	rm -f *.o