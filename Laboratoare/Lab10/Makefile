CC=mpicc
MPI=mpirun
MPIFLAGS=-lm

NP_Bcast=198
NP_Reduce_Scan=16

.PHONY: build clean

build: Bcast Reduce Scan

Bcast: Bcast.c
	$(CC) $^ -o $@ $(MPIFLAGS)

Reduce: Reduce.c
	$(CC) $^ -o $@

Scan: Scan.c
	$(CC) $^ -o $@

run_Bcast: Bcast
	$(MPI) -np $(NP_Bcast) $<

run_Reduce: Reduce
	$(MPI) -np $(NP_Reduce_Scan) $<

run_Scan: Scan
	$(MPI) -np $(NP_Reduce_Scan) $< | sort -t ' ' -k3 -n

clean:
	rm -f Bcast Reduce Scan .fuse_hidden*