CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) producent.c -o producent	
	$(CC) $(CFLAGS) client.c -o client	
	$(CC) $(CFLAGS) main.c -o main	
	rm -f *.o

test:
	make static		
	./main
	


clean:
	rm -f *.o *.a *.so find
