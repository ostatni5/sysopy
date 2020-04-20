CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) macierz.c -o macierz 
	$(CC) $(CFLAGS) generate.c -o generate 
	$(CC) $(CFLAGS) validate.c -o validate 
	rm -f *.o

test:
	make static
	./generate 10 5 10
	./macierz list 5 10 1
	./validate list
	./macierz list 5 10 2
	./validate list
	

clean:
	rm -f *.o *.a *.so find