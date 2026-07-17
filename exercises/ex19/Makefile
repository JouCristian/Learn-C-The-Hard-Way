CC=gcc

CFLAGS=-Wall -Wextra -g

all: ex19

ex19: ex19.o object.o
	$(CC) $(CFLAGS) ex19.o object.o -o ex19

ex19.o: ex19.c ex19.h object.h
	$(CC) $(CFLAGS) -c ex19.c -o ex19.o

object.o: object.c object.h
	$(CC) $(CFLAGS) -c object.c -o object.o

clean:
	rm -f ex19 ex19.o object.o

.PHONY: all clean
