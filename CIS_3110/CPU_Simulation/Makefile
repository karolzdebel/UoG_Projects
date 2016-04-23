CFLAGS = -std=c99 -Wall -pedantic
CC = gcc

simcpu: simcpu.o list.o

cpusim.o: simcpu.c simcpu.h

list.o: list.c list.h

clean: 
	rm -rf *.o simcpu
