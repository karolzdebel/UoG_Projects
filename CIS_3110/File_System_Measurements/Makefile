CC = gcc

all: filetest

filetest: filetest.o
	$(CC) -g -std=c99 -o filetest filetest.o

filetest.o: filetest.c 
	$(CC) -g -std=c99 -c filetest.c

clean:
	rm -f *.o filetest
