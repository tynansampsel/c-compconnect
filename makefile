CC=gcc
CFLAGS=-Wall -Wextra

all: main

main: main.c cookiejar.c cookieeater.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f main