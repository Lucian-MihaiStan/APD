CC=gcc
CFLAGS=-lpthread -lm

NMemset=1073741824
NReps=100
printLevel=0
P=12  # sarakilor

.PHONY: clean all

all: memset sqrt

memset: memset.c
	$(CC) $? -o $@ $(CFLAGS)

sqrt: sqrt.c
	$(CC) $? -o $@

run_memset: memset
	time ./$< $(NMemset) $(NReps) $(P) $(printLevel)

run_sqrt: sqrt
	time ./$<

clean:
	rm -f memset sqrt

