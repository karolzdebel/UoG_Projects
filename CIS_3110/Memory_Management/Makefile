CFLAGS = -Wall -pedantic -std=c99 -g -pthread
CC = gcc

all: holes

holes: list.o part2.o
	$(CC) $(CFLAGS) -o holes part2.o list.o

part2.o: part2.c part2.h

list.o: list.c list.h

clean:
	rm -f *.o holes
