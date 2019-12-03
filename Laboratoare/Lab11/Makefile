CC=mpicc
MPI=mpirun
MPIFLAGS=-lm

NP_NonBlock=2
NP_GraphUtils=10

.PHONY: build clean

build: NonBlock GraphUtils SendIsBlocking
 
NonBlock: NonBlock.c
	$(CC) $^ -o $@

GraphUtils: mpiGraph.c
	$(CC) $^ -o $@

SendIsBlocking: sendIsBlocking.c
	$(CC) $^ -o $@

Poly: polynomialFunction.c
	$(CC) $^ -o $@

Sort: sort.c
	$(CC) $^ -o $@

RankSort: RankSort.c
	$(CC) $^ -o $@ $(MPIFLAGS)

run_NonBlock: NonBlock
	$(MPI) -np $(NP_NonBlock) $<

run_GraphUtils: GraphUtils
	$(MPI) -np $(NP_GraphUtils) $<

run_SendIsBlocking: SendIsBlocking
	$(MPI) -np $(NP_NonBlock) $<

run_Poly: Poly
	$(MPI) -np $(NP_Poly) $<

run_Sort: Sort
	$(MPI) -np $(NP_Sort) $<

run_RankSort: RankSort
	$(MPI) -np $(NP_RankSort) $<

clean:
	rm -f NonBlock GraphUtils SendIsBlocking .fuse_hidden*