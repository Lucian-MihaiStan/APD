CC=gcc
CFLAGS=-lm -lpthread -O3

.PHONY: build clean run

build: helloWorld

addVectors: addVectors.c
	gcc -o $@ $^ $(CFLAGS)

run: helloWorld
	./helloWorld

clean:
	rm helloWorld