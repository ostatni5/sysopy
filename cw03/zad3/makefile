CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) macierz.c -o macierz 
	$(CC) $(CFLAGS) generate.c -o generate 
	rm -f *.o

test:
	make static
	./generate 10 5 10
	@echo "-------OPTION 1"
	./macierz list 5 10 1 5 1024	
	@echo "-------OPTION 2"
	./macierz list 5 10 2 5 1024
	

clean:
	rm -f *.o *.a *.so find