.PHONY: build clean run

build: helloWorld

helloWorld: helloWorld.c
	gcc -o helloWorld helloWorld.c -lm -O3 -lpthread

run: helloWorld
	./helloWorld

clean:
	rm helloWorld