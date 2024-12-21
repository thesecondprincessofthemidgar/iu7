CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g

.PHONY: all clean

all: main

main: main.o tools.o
	$(CC) $(CFLAGS) -o app.exe main.o tools.o

main.o: main.c tools.h
	$(CC) $(CFLAGS) -c main.c

tools.o: tools.c tools.h
	$(CC) $(CFLAGS) -c tools.c

clean:
	rm -f *.o main
