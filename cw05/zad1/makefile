CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) main.c -o main	
	$(CC) $(CFLAGS) chain.c -o chain	
	rm -f *.o

test:
	make static
	@echo START > FILENAME 
	./main commands 0

clean:
	rm -f *.o *.a *.so find