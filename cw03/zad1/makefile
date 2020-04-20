CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) find.c -o find 
	rm -f *.o

test:
	make static
	./find . 
	

clean:
	rm -f *.o *.a *.so find