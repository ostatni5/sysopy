CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) main.c -o main	
	rm -f *.o

test:
	make static
	./main commands

clean:
	rm -f *.o *.a *.so find