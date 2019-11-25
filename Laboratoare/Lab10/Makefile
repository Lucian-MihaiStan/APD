CC=mpicc
MPI=mpirun
MPIFLAGS=-lm

NP_Bcast=198
NP_Reduce_Scan=16
NP_Poly=14
NP_Sort=100
NP_RankSort=10

.PHONY: build clean

build: Bcast Reduce Scan Poly Sort RankSort
 
Bcast: Bcast.c
	$(CC) $^ -o $@ $(MPIFLAGS)

Reduce: Reduce.c
	$(CC) $^ -o $@

Scan: Scan.c
	$(CC) $^ -o $@

Poly: polynomialFunction.c
	$(CC) $^ -o $@

Sort: sort.c
	$(CC) $^ -o $@

RankSort: RankSort.c
	$(CC) $^ -o $@ $(MPIFLAGS)

run_Bcast: Bcast
	$(MPI) -np $(NP_Bcast) $<

run_Reduce: Reduce
	$(MPI) -np $(NP_Reduce_Scan) $<

run_Scan: Scan
	$(MPI) -np $(NP_Reduce_Scan) $< | sort -t ' ' -k3 -n

run_Poly: Poly
	$(MPI) -np $(NP_Poly) $<

run_Sort: Sort
	$(MPI) -np $(NP_Sort) $<

run_RankSort: RankSort
	$(MPI) -np $(NP_RankSort) $<

clean:
	rm -f Bcast Reduce Scan Poly Sort RankSort .fuse_hidden*