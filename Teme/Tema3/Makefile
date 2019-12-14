CC=mpicc
GCC=gcc
MPI=mpirun
MPIFLAGS=-lm

NP=1

.PHONY: build clean

build: tema3
 
tema3: tema3.o pnm_image_utils.o
	$(CC) $^ -o $@ $(MPIFLAGS)

tema3.o: tema3.c
	$(CC) $^ -c

pnm_image_utils.o: pnm_image_utils.c
	$(CC) $^ -c

run_tema3: tema3
	$(MPI) -np $(NP) $<

run_tema3_gcc: tema3
	./$<

clean:
	rm -f tema3 *.o .fuse_hidden* *.pgm *.pnm