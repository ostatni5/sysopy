CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) main.c -o main -lrt
	$(CC) $(CFLAGS) client.c -o client -lrt
	rm -f *.o

test:
	make static
	./main

clean:
	rm -f *.o *.a *.so find