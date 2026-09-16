CC = gcc
CFLAGS = -Wall -Wextra

all: signal1 signal2 timer

signal1: signal1.c
	$(CC) $(CFLAGS) -o signal1 signal1.c

signal2: signal2.c
	$(CC) $(CFLAGS) -o signal2 signal2.c

timer: timer.c
	$(CC) $(CFLAGS) -o timer timer.c

clean:
	rm -f signal1 signal2 timer