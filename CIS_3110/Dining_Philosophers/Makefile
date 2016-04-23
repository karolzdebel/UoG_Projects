CFLAGS = -Wall -pedantic -std=c99 -g -pthread
CC = gcc

all: part1.o
	$(CC) $(CFLAGS) -o dine part1.o

part1.out: part1.c 
	$(CC) $(CFLAGS) -c part1.c

clean:
	rm -f *.o dine
