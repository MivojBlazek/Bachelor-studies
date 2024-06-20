CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic

default: lab5

all: lab5 test
	
lab5: main.o functions.o
	$(CC) $(CFLAGS) -o $@ $^

test: test.o functions.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c -o $@ $<

test.o: test.c functions.h
	$(CC) $(CFLAGS) -c -o $@ $<

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o

cleanall: clean
	rm -f lab5

pack: 
	mkdir xblaze38.zip | 

upload: 
	

run: 
	

run_test: 
	

valgrind: 
	

leaks: 
	

.PHONY: default clean
