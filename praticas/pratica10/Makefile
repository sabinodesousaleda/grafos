CC = gcc
CFLAGS = -Wall -Wextra -O2

all: main

main: main.c hamiltoniano.c tps.c hamiltoniano.h tps.h
	$(CC) $(CFLAGS) -o main main.c hamiltoniano.c tps.c

clean:
	rm -f main main.exe
