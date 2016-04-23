CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g

all: shell.o program.o
	$(CC) $(CFLAGS) -o CIS3110sh shell.o program.o

shell.o: shell.c shell.h
	$(CC) $(CFLAGS) -c shell.c

program.o: program.c
	$(CC) $(CFLAGS) -c program.c

clean:
	rm *.o CIS3110sh
