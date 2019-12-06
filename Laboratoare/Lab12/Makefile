CC=gcc
CFLAGS=-lpthread -lm

NMemset=1073741824
NReps=100
printLevel=0
P=12  # sarakilor

.PHONY: clean all

all: memset

memset: memset.c
	$(CC) $? -o $@ $(CFLAGS)

run_memset: memset
	time ./$< $(NMemset) $(NReps) $(P) $(printLevel)

clean:
	rm -f memset

