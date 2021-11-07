CC=gcc
CFLAGS=-pthread -Wall

clean:
	rm -rf *.o
	rm -rf dpp

dpp: dpp.o
	$(CC) $(CFLAGS) -o dpp dpp.o

dpp.o: dpp.c
	$(CC) $(CFLAGS) -c dpp.c
