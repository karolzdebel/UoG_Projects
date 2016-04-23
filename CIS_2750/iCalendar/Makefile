CC = gcc
CFLAGS = -fPIC `pkg-config --cflags python3` -Wall -std=c11 -DNDEBUG -g
LDFLAGS = 

all: caltool Cal.so

caltool: caltool.o calutil.o
	$(CC) $(CFLAGS) -o caltool caltool.o calutil.o

caltool.o: caltool.c caltool.h
	$(CC) $(CFLAGS) -c caltool.c

calutil.o: calutil.c calutil.h
	$(CC) $(CFLAGS) -c calutil.c

Cal.so: caltool.o calmodule.o calutil.o
	$(CC) -shared $^ $(LDFLAGS) -o $@

calmodule.o: calmodule.c calutil.h

clean:
	rm -f *.o *.so caltool
