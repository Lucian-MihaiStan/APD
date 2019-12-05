CC=mpicc
MPI=mpirun
MPIFLAGS=-lm

NP_NonBlock=2
NP_Graph=10

.PHONY: build clean

build: NonBlock GraphUtils SendIsBlocking Topology Send0To9 FindLeader CountNodes
 
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

CountNodes: CountNodes.c
	$(CC) $^ -o $@ $(MPIFLAGS)

FindLeader: FindLeader.c
	$(CC) $^ -o $@

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

run_FindLeader: FindLeader
	$(MPI) -np $(NP_Graph) $<

run_CountNodes: CountNodes
	$(MPI) -np $(NP_Graph) $<

clean:
	rm -f NonBlock GraphUtils SendIsBlocking Topology Send0To9 FindLeader CountNodes .fuse_hidden*