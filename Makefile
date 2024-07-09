CC=gcc
CFLAGS=-std=c2x -Wall -Wpedantic -Wextra -fPIC

.PHONY: all

all: main libsha3.so test

main: sha3.o test.c
	$(CC) $(CFLAGS) -o main test.c sha3.o

sha3.o: sha3.c sha3.h
	$(CC) $(CFLAGS) -o sha3.o -c sha3.c

libsha3.so: sha3.o
	$(CC) $(CFLAGS) -o libsha3.so -shared sha3.o

test: main libsha3.so
	./main && ./run_test.py

clean:
	rm -f *.o *.so main
	rm -rf output