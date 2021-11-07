CC=gcc
CFLAGS=-Wall

clean:
	rm -rf *.o
	rm -rf allocator

allocator: allocator.o
	$(CC) $(CFLAGS) -o allocator allocator.o

allocator.o: allocator.c
	$(CC) $(CFLAGS) -c allocator.c
