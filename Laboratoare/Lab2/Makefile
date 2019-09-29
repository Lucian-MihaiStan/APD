CC=gcc
CFLAGS=-lm -lpthread
DEBUG=-g

.PHONY: build clean

build: helloWorld addVectors allCores raceCondition oneTwo

addVectors: addVectors.c
	$(CC) -o $@ $^ $(CFLAGS) $(DEBUG)

helloWorld: helloWorld.c
	$(CC) -o $@ $^ $(CFLAGS) $(DEBUG) -O3

allCores: allCores.c
	$(CC) -o $@ $^ $(CFLAGS)

raceCondition: raceCondition.c
	$(CC) -o $@ $^ $(CFLAGS)

oneTwo: oneTwo.c
	$(CC) -o $@ $^ $(CFLAGS)

run_hello: helloWorld
	./$<

run_add: addVectors
	time ./$< $(N) $(NReps) $(P) $(printLevel)

run_cores: allCores
	./$<

run_one: oneTwo
	./$<

run_race: raceCondition
	./testCorrectnessIntensive.sh $< $(N)

clean:
	rm -f helloWorld addVectors allCores raceCondition oneTwo