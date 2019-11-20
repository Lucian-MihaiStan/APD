CC=mpicc
MPI=mpirun
MPIFLAGS=-lm

NP_Bcast=198
NP_Reduce=16

.PHONY: build clean

build: Bcast Reduce

Bcast: Bcast.c
	$(CC) $^ -o $@ $(MPIFLAGS)

Reduce: Reduce.c
	$(CC) $^ -o $@

run_Bcast: Bcast
	$(MPI) -np $(NP_Bcast) $<

run_Reduce: Reduce
	$(MPI) -np $(NP_Reduce) $<

clean:
	rm -f Bcast Reduce .fuse_hidden*