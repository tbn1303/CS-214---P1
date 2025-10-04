CC = gcc
CFLAGS = -g -Wall -Wvla -std=c99 -fsanitize=address,undefined


all: memtest memgrind

memtest: memtest.o mymalloc.o
        $(CC) $(CFLAGS) $^ -o $@

memgrind: memgrind.o mymalloc.o
        $(CC) $(CFLAGS) $^ -o $@

%.0: %.c
        $(CC) $(CFLAGS) -c $<

mymalloc.o: mymalloc.h
memtest.o: mymalloc.h
memgrind.o: mymalloc.h

clean:
        rm -f *.o memtest memgrind
