CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) catcher.c -o catcher
	$(CC) $(CFLAGS) sender.c -o sender
	rm -f *.o


clean:
	rm -f *.o *.a *.so find