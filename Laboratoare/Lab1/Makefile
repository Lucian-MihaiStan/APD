.PHONY: build clean run

build: helloWorld

addVectors: addVectors.c
	gcc -o $@ $^ -lm -O3 -lpthread

run: helloWorld
	./helloWorld

clean:
	rm helloWorld