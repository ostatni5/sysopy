CC=gcc
CFLAGS= -Wall -g

static:
	make clean
	$(CC) $(CFLAGS) main.c -o main 
	rm -f *.o

runTest:
	make static
	echo "TestStart" > allTimes.txt
	# 1, 4, 512, 1024, 4096 i 8192 
	make doubleTest length=1
	make doubleTest length=4
	make doubleTest length=512
	make doubleTest length=1024
	make doubleTest length=4096
	make doubleTest length=8192	

doubleTest3:	
	make singleTest count=3000 length=10

doubleTest:	
	make singleTest count=8000 length=${length}
	make singleTest count=15000 length=${length}
singleTest:
	echo "Test count: ${count} length: ${length}\n" >> allTimes.txt
	./main generate data ${count} ${length}
	cat times.txt >> allTimes.txt
	./main copy data dataSortSys ${count} ${length} sys
	cat times.txt >> allTimes.txt
	./main copy data dataSortLib ${count} ${length} lib
	cat times.txt >> allTimes.txt
	./main sort dataSortSys ${count} ${length} sys
	cat times.txt >> allTimes.txt
	./main sort dataSortLib ${count} ${length} lib
	cat times.txt >> allTimes.txt
	echo "\n\n" >> allTimes.txt
	


clean:
	rm -f *.o *.a *.so main