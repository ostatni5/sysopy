CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) find.c -o find 
	rm -f *.o

test:
	make static
	./find . "*" > ree.txt
test2:
	make static
	./find ../.. "*" -maxdepth 0 -mtime +0 > ree.txt
	./find ../.. "a" -maxdepth 1 -atime -2 > ree2.txt
	./find ../.. "*" -maxdepth 3 -atime +1 > ree3.txt

clean:
	rm -f *.o *.a *.so find