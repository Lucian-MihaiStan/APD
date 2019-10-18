CC=gcc
CFLAGS=-lpthread -lm

NBubble=100000
NMerge=67100000
NShear=5000
NReps=1
printLevel=0
P=12 # sarakilor
PMerge=8

.PHONY: clean all

all: two_sorts mergeSort parShear seqShear bubbleSort

two_sorts: two_sorts.c
	$(CC) $? -o $@ $(CFLAGS)

mergeSort: mergeSort.c
	$(CC) $? -o $@ $(CFLAGS)

bubbleSort: bubbleSort.c
	$(CC) $? -o $@ $(CFLAGS)

seqShear: seq_shearSort.c
	$(CC) $? -o $@ $(CFLAGS)

parShear: par_shearSort.c
	$(CC) $? -o $@ $(CFLAGS)

run_two_sorts: two_sorts
	./$<

run_mergeSort: mergeSort
	time ./$< $(NMerge) $(PMerge) $(printLevel)

run_bubbleSort: bubbleSort
	time ./$< $(NBubble) $(P) $(printLevel)

run_seqShear: seqShear
	time ./$< $(NShear) $(printLevel)

run_parShear: parShear
	time ./$< $(NShear) $(P) $(printLevel)

clean:
	rm -f two_sorts mergeSort bubbleSort parShear seqShear

