CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I inc/ -g

build: src/main.c inc/list_tools.h
	$(CC) $(CFLAGS) src/main.c src/list_tools.c -o app.exe

run: build
	./app.exe

clean:
	rm -f app.exe
