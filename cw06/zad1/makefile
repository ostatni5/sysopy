CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) main.c -o main	
	$(CC) $(CFLAGS) client.c -o client	
	rm -f *.o

test:
	make static
	./main

clean:
	rm -f *.o *.a *.so find