CC = gcc
CFLAGS = -g -Wall -Wvla -std=c99 -fsanitize=address,undefined

all: memtest memgrind

memtest: memtest.o mymalloc.o
	$(CC) $(CFLAGS) $^ -o $@

memgrind: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) $^ -o $@

%.0: %.c
	$(CC) $(CFLAGS) -c $<

memgrind.o memtest.o mymalloc.o: mymalloc.h

clean:
	rm -f *.o memtest memgrind
    # FOR WINDOWS: del /Q memgrind.exe memtest.exe *.o 2>nul || echo Clean complete
