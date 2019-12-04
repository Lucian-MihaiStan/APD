CC=mpicc
MPI=mpirun
MPIFLAGS=-lm

NP_NonBlock=2
NP_Graph=10

.PHONY: build clean

build: NonBlock GraphUtils SendIsBlocking Topology Send0To9
 
NonBlock: NonBlock.c
	$(CC) $^ -o $@

GraphUtils: mpiGraph.c
	$(CC) $^ -o $@

SendIsBlocking: sendIsBlocking.c
	$(CC) $^ -o $@

Topology: Topology.c
	$(CC) $^ -o $@

Send0To9: Send0To9.c
	$(CC) $^ -o $@

RankSort: RankSort.c
	$(CC) $^ -o $@ $(MPIFLAGS)

run_NonBlock: NonBlock
	$(MPI) -np $(NP_NonBlock) $<

run_GraphUtils: GraphUtils
	$(MPI) -np $(NP_Graph) $<

run_SendIsBlocking: SendIsBlocking
	$(MPI) -np $(NP_NonBlock) $<

run_Topology: Topology
	$(MPI) -np $(NP_Graph) $<

run_Send0To9: Send0To9
	$(MPI) -np $(NP_Graph) $<

run_RankSort: RankSort
	$(MPI) -np $(NP_RankSort) $<

clean:
	rm -f NonBlock GraphUtils SendIsBlocking Topology NP_Graph Send0To9 .fuse_hidden*