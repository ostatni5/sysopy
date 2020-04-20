CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) main.c -o main
	$(CC) $(CFLAGS) child.c -o child
	rm -f *.o

test:
	make static
	echo "---------"  > res.txt
	./main 0 >> res.txt
	echo "---------"  >> res.txt
	./main 1 >> res.txt
	echo "---------"  >> res.txt
	./main 2 >> res.txt
	echo "---------"  >> res.txt
	./main 3 >> res.txt

clean:
	rm -f *.o *.a *.so find